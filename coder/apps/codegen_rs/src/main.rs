//! Code generator that reads the hand-written `robot.rs` message definitions
//! and emits Rust + C++ using the existing Tera templates.
//!
//! `robot.rs` supplies the message structs (name, fields, and `//` doc
//! comments). Endpoint and robot-level config (multicast port/address, endpoint
//! list) cannot be expressed in a Rust struct file, so those are still read
//! from `robot.hcl`. If `robot.hcl` is missing, generation continues with
//! default robot config and no endpoints.
//!
//! Usage:
//!   cargo run --bin codegen_rs \
//!     -i hcl/robot.rs \
//!     -c hcl/robot.hcl \
//!     -g rust.tera ../generated/src/generated.rs \
//!     -g cpp_header.tera ../generated/cpp/generated.h \
//!     -g cpp_source.tera ../generated/cpp/generated.cpp

use anyhow::Context;
use chrono::Local;
use clap::Parser;
use common::{fnv1a_32, load_robot_config, RobotConfig, SubscribeConfig};
use serde::Serialize;
use std::fs;
use std::path::Path;
use tera::{Context as TeraContext, Tera};

// ── CLI ───────────────────────────────────────────────────────────────────

#[derive(Parser)]
#[command(about = "Generate code from hand-written robot.rs message definitions")]
struct Args {
    /// Path to the robot.rs file containing the message structs.
    #[arg(short = 'i', long, default_value = "hcl/robot.rs")]
    input: String,

    /// Path to robot.hcl, used only for endpoint and robot-level config.
    #[arg(short = 'c', long, default_value = "hcl/robot.hcl")]
    config: String,

    /// Template directory.
    #[arg(short = 'd', long, default_value = "hcl")]
    template_dir: String,

    /// Template → output pairs (repeatable): -g TEMPLATE OUTPUT
    #[arg(
        short = 'g',
        long = "gen",
        num_args = 2,
        value_names = ["TEMPLATE", "OUTPUT"],
        required = true
    )]
    generators: Vec<String>,
}

// ── Template data model (mirrors apps/codegen) ─────────────────────────────

#[derive(Serialize)]
struct TemplateCtx {
    build_date: String,
    robot: RobotCtx,
    messages: Vec<MessageCtx>,
    endpoints: Vec<EndpointCtx>,
}

#[derive(Serialize)]
struct RobotCtx {
    name: String,
    id: u32,
    model: String,
    description: String,
    multicast_port: u16,
    multicast_addr: String,
}

#[derive(Serialize, Clone)]
struct EndpointCtx {
    name: String,
    id: u32,
    const_name: String,
    interfaces: Vec<String>,
    subscribes: Vec<SubscribeCtx>,
    description: String,
}

#[derive(Serialize, Clone)]
struct SubscribeCtx {
    src: String,
    msg_type: String,
    dst: String,
}

#[derive(Serialize)]
struct MessageCtx {
    name: String,
    id: u32,
    fields: Vec<FieldCtx>,
}

#[derive(Serialize)]
struct FieldCtx {
    name: String,
    id: u32,
    rust_type: String,
    cpp_type: String,
    description: String,
    unit: String,
    index: usize,
}

// ── robot.rs parser ────────────────────────────────────────────────────────

/// A message struct parsed from `robot.rs`.
struct RsMessage {
    name: String,
    fields: Vec<RsField>,
}

/// A single field line parsed from `robot.rs`.
struct RsField {
    name: String,
    /// Rust type exactly as written (e.g. `Option<String>` or `Vec<u32>`).
    rust_type: String,
    /// Text after a trailing `//` comment, if any.
    description: String,
}

/// Parse `robot.rs` source into message structs.
///
/// Assumes the hand-written layout: `struct Name {` on its own line, one field
/// per line, closing `}` on its own line.
fn parse_robot_rs(src: &str) -> Vec<RsMessage> {
    let mut messages = Vec::new();
    let mut current: Option<RsMessage> = None;

    for line in src.lines() {
        let trimmed = line.trim();

        if trimmed.is_empty() || trimmed.starts_with("//") {
            continue;
        }

        if let Some(rest) = trimmed.strip_prefix("struct ") {
            let name = rest.split('{').next().unwrap_or("").trim().to_string();
            if !name.is_empty() {
                current = Some(RsMessage {
                    name,
                    fields: Vec::new(),
                });
            }
            continue;
        }

        if trimmed.starts_with('}') {
            if let Some(msg) = current.take() {
                messages.push(msg);
            }
            continue;
        }

        if let Some(msg) = current.as_mut() {
            if let Some(field) = parse_field(trimmed) {
                msg.fields.push(field);
            }
        }
    }

    if let Some(msg) = current.take() {
        messages.push(msg);
    }
    messages
}

/// Parse a single `name: Type, // comment` field line.
fn parse_field(line: &str) -> Option<RsField> {
    let (decl, description) = match line.split_once("//") {
        Some((d, c)) => (d.trim(), c.trim().to_string()),
        None => (line.trim(), String::new()),
    };

    let (name, rest) = decl.split_once(':')?;
    let name = name.trim().to_string();
    if name.is_empty() {
        return None;
    }

    let rust_type = rest.trim().trim_end_matches(',').trim().to_string();
    if rust_type.is_empty() {
        return None;
    }

    Some(RsField {
        name,
        rust_type,
        description,
    })
}

// ── Type mapping ───────────────────────────────────────────────────────────

/// The templates wrap every field in `Option<T>` themselves, so return the
/// *inner* Rust type: `Option<String>` → `String`, `Vec<u32>` → `Vec<u32>`.
fn inner_rust_type(ty: &str) -> String {
    ty.strip_prefix("Option<")
        .and_then(|s| s.strip_suffix('>'))
        .unwrap_or(ty)
        .to_string()
}

/// Map an inner Rust type to the C++ type used by the cpp templates.
fn rust_to_cpp(rust: &str) -> anyhow::Result<String> {
    if let Some(inner) = rust.strip_prefix("Vec<").and_then(|s| s.strip_suffix('>')) {
        return Ok(format!("std::vector<{}>", rust_scalar_to_cpp(inner)?));
    }
    rust_scalar_to_cpp(rust)
}

fn rust_scalar_to_cpp(rust: &str) -> anyhow::Result<String> {
    let cpp = match rust {
        "bool" => "bool",
        "i8" => "int8_t",
        "i16" => "int16_t",
        "i32" => "int32_t",
        "i64" => "int64_t",
        "u8" => "uint8_t",
        // CBOR has no 16-bit integer type; widen to uint32_t so the existing
        // cpp encode/decode branches handle it.
        "u16" => "uint32_t",
        "u32" => "uint32_t",
        "u64" => "uint64_t",
        "f32" => "float",
        "f64" => "double",
        "String" => "std::string",
        other => anyhow::bail!("unsupported Rust type: {other}"),
    };
    Ok(cpp.to_string())
}

// ── Context building ───────────────────────────────────────────────────────

fn build_message_ctx(msg: RsMessage) -> anyhow::Result<MessageCtx> {
    let id = fnv1a_32(&msg.name);
    let fields = msg
        .fields
        .into_iter()
        .enumerate()
        .map(|(i, f)| {
            let rust_type = inner_rust_type(&f.rust_type);
            let cpp_type = rust_to_cpp(&rust_type)?;
            Ok(FieldCtx {
                name: f.name,
                id: i as u32,
                rust_type,
                cpp_type,
                description: f.description,
                unit: String::new(),
                index: i,
            })
        })
        .collect::<anyhow::Result<Vec<_>>>()?;
    Ok(MessageCtx {
        name: msg.name,
        id,
        fields,
    })
}

fn build_template_context(
    rs_messages: Vec<RsMessage>,
    robot: RobotCtx,
    endpoints: Vec<EndpointCtx>,
) -> anyhow::Result<TeraContext> {
    let mut messages = rs_messages
        .into_iter()
        .map(build_message_ctx)
        .collect::<anyhow::Result<Vec<_>>>()?;
    messages.sort_by(|a, b| a.name.cmp(&b.name));

    let template_ctx = TemplateCtx {
        build_date: Local::now().format("%Y-%m-%d %H:%M:%S").to_string(),
        robot,
        messages,
        endpoints,
    };
    Ok(TeraContext::from_serialize(&template_ctx)?)
}

fn robot_to_ctx(cfg: &RobotConfig) -> RobotCtx {
    RobotCtx {
        id: fnv1a_32(&cfg.name),
        name: cfg.name.clone(),
        model: cfg.model.clone(),
        description: cfg.description.clone().unwrap_or_default(),
        multicast_port: cfg.multicast_port,
        multicast_addr: cfg
            .multicast_addr
            .clone()
            .unwrap_or_else(|| "224.0.0.1".to_string()),
    }
}

fn default_robot_ctx() -> RobotCtx {
    let name = String::new();
    let id = fnv1a_32(&name);
    RobotCtx {
        name,
        id,
        model: String::new(),
        description: String::new(),
        multicast_port: 0,
        multicast_addr: "224.0.0.1".to_string(),
    }
}

fn endpoints_to_ctx(cfg: &RobotConfig) -> Vec<EndpointCtx> {
    let mut endpoints: Vec<EndpointCtx> = cfg
        .endpoints
        .iter()
        .map(|(name, ep)| EndpointCtx {
            id: fnv1a_32(name),
            name: name.clone(),
            const_name: to_const_ident(name),
            interfaces: ep.interfaces.clone(),
            subscribes: ep.subscribes.iter().map(subscribe_to_ctx).collect(),
            description: ep.description.clone().unwrap_or_default(),
        })
        .collect();
    endpoints.sort_by(|a, b| a.const_name.cmp(&b.const_name));
    endpoints
}

fn subscribe_to_ctx(sub: &SubscribeConfig) -> SubscribeCtx {
    SubscribeCtx {
        src: sub.src.clone().unwrap_or_default(),
        msg_type: sub.msg_type.clone().unwrap_or_default(),
        dst: sub.dst.clone().unwrap_or_default(),
    }
}

fn to_const_ident(s: &str) -> String {
    let mut out = String::with_capacity(s.len());
    let mut prev_sep = true;
    for ch in s.chars() {
        if ch.is_ascii_alphanumeric() {
            if ch.is_ascii_lowercase() {
                out.push(ch.to_ascii_uppercase());
            } else {
                out.push(ch);
            }
            prev_sep = false;
        } else if !prev_sep {
            out.push('_');
            prev_sep = true;
        }
    }
    out.trim_end_matches('_').to_string()
}

fn render_template(
    tera: &Tera,
    template_name: &str,
    ctx: &TeraContext,
) -> anyhow::Result<String> {
    let mut render_context = ctx.clone();
    render_context.insert("template_name", template_name);
    tera.render(template_name, &render_context)
        .with_context(|| format!("template rendering failed for {template_name}"))
}

// ── Main ───────────────────────────────────────────────────────────────────

fn main() -> anyhow::Result<()> {
    let args = Args::parse();

    // 1. Parse robot.rs for message definitions.
    let rs_src = fs::read_to_string(&args.input)
        .with_context(|| format!("failed to read {}", args.input))?;
    let rs_messages = parse_robot_rs(&rs_src);
    if rs_messages.is_empty() {
        anyhow::bail!(
            "no message structs found in {} — expected a robot.rs file with `struct` definitions, not an HCL file",
            args.input
        );
    }
    eprintln!("parsed {} messages from {}", rs_messages.len(), args.input);

    // 2. Load robot.hcl for endpoint + robot config (best effort).
    let (robot, endpoints) = match load_robot_config(&args.config) {
        Ok(cfg) => (robot_to_ctx(&cfg), endpoints_to_ctx(&cfg)),
        Err(e) => {
            eprintln!("warning: {e}; using default robot config and no endpoints");
            (default_robot_ctx(), Vec::new())
        }
    };

    // 3. Build template context.
    let tera_context = build_template_context(rs_messages, robot, endpoints)?;

    // 4. Render each template → output pair.
    let tera = Tera::new(&format!("{}/{}", &args.template_dir, "*.tera"))
        .with_context(|| format!("failed to load templates from {}", &args.template_dir))?;

    for pair in args.generators.chunks(2) {
        let template = &pair[0];
        let output = &pair[1];
        let code = render_template(&tera, template, &tera_context)
            .with_context(|| format!("template rendering failed for {template}"))?;
        if let Some(parent) = Path::new(output).parent().filter(|p| !p.as_os_str().is_empty()) {
            fs::create_dir_all(parent)
                .with_context(|| format!("failed to create output directory {parent:?}"))?;
        }
        fs::write(output, &code).with_context(|| format!("failed to write {output}"))?;
        let ext = template.split('.').next().unwrap_or(template);
        eprintln!("{ext:>8} -> {output}");
    }

    Ok(())
}
