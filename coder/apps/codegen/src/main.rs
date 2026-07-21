//! Code generator: reads an HCL robot config and emits code via Tera templates.
//!
//! Usage:
//!   cargo run --bin codegen \
//!     -g rust.tera ../generated/src/generated.rs \
//!     -g cpp_header.tera ../generated/cpp/generated.h \
//!     -g cpp_source.tera ../generated/cpp/generated.cpp
//!
//!   cargo run --bin codegen \
//!     -i hcl/robot.hcl \
//!     -g rust.tera ../generated/src/generated.rs

use anyhow::Context;
use clap::Parser;
use common::{fnv1a_32, load_robot_config, MessageConfig, RobotConfig, SubscribeConfig};
use serde::Serialize;
use tera::{Context as TeraContext, Tera};

#[derive(Parser)]
#[command(about = "Generate code from an HCL robot config")]
struct Args {
    /// Path to the HCL robot config
    #[arg(short = 'i', long, default_value = "../../hcl/robot.hcl")]
    input: String,

    /// Template directory
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

fn main() -> anyhow::Result<()> {
    let args = Args::parse();

    // ── Load config and build template context once ──────────────────────
    let cfg = load_robot_config(&args.input)
        .with_context(|| format!("failed to load {}", args.input))?;
    let tera_context = build_template_context(&cfg)?;
    let tera = Tera::new(&format!("{}/{}", &args.template_dir, "*.tera"))
        .with_context(|| format!("failed to load templates from {}", &args.template_dir))?;

    // ── Iterate over template → output pairs ─────────────────────────────
    for pair in args.generators.chunks(2) {
        let template = &pair[0];
        let output = &pair[1];

        let code = render_template(&tera, template, &tera_context)
            .with_context(|| format!("template rendering failed for {template}"))?;
        std::fs::write(output, &code)
            .with_context(|| format!("failed to write {output}"))?;

        let ext = template.split('.').next().unwrap_or(template);
        eprintln!("{ext:>8} -> {output}");
    }

    let msg_count = tera_context
        .get("messages")
        .and_then(|v| v.as_array())
        .map(|a| a.len())
        .unwrap_or(0);
    let ep_count = tera_context
        .get("endpoints")
        .and_then(|v| v.as_array())
        .map(|a| a.len())
        .unwrap_or(0);
    eprintln!(
        "Done: {} messages, {} endpoints → {} file(s)",
        msg_count,
        ep_count,
        args.generators.len() / 2
    );

    Ok(())
}

/// Render a single template with the given context.
fn render_template(
    tera: &Tera,
    template_name: &str,
    ctx: &TeraContext,
) -> anyhow::Result<String> {
    tera.render(template_name, ctx)
        .with_context(|| format!("template rendering failed for {template_name}"))
}

// ── Template data model ─────────────────────────────────────────────────

#[derive(Serialize)]
struct TemplateCtx {
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

fn build_template_context(cfg: &RobotConfig) -> anyhow::Result<TeraContext> {
    let mut messages: Vec<MessageCtx> = Vec::new();
    let mut endpoints: Vec<EndpointCtx> = Vec::new();

    let mut msg_names: Vec<&String> = cfg.messages.keys().collect();
    msg_names.sort();
    for msg_name in msg_names {
        messages.push(message_to_ctx(msg_name, &cfg.messages[msg_name])?);
    }

    let mut ep_names: Vec<&String> = cfg.endpoints.keys().collect();
    ep_names.sort();
    for ep_name in ep_names {
        let ep = &cfg.endpoints[ep_name];
        endpoints.push(EndpointCtx {
            name: ep_name.clone(),
            id: fnv1a_32(ep_name),
            const_name: to_const_ident(ep_name),
            interfaces: ep.interfaces.clone(),
            subscribes: ep.subscribes.iter().map(subscribe_to_ctx).collect(),
            description: ep.description.clone().unwrap_or_default(),
        });
    }
    endpoints.sort_by(|a, b| a.const_name.cmp(&b.const_name));

    let template_ctx = TemplateCtx {
        robot: RobotCtx {
            name: cfg.name.clone(),
            id: fnv1a_32(&cfg.name),
            model: cfg.model.clone(),
            description: cfg.description.clone().unwrap_or_default(),
            multicast_port: cfg.multicast_port,
            multicast_addr: cfg
                .multicast_addr
                .clone()
                .unwrap_or("224.0.0.1".to_string()),
        },
        messages,
        endpoints,
    };
    let ctx = TeraContext::from_serialize(&template_ctx)?;
    Ok(ctx)
}

fn subscribe_to_ctx(sub: &SubscribeConfig) -> SubscribeCtx {
    SubscribeCtx {
        src: sub.src.clone().unwrap_or_default(),
        msg_type: sub.msg_type.clone().unwrap_or_default(),
        dst: sub.dst.clone().unwrap_or_default(),
    }
}

fn message_to_ctx(name: &str, msg: &MessageConfig) -> anyhow::Result<MessageCtx> {
    let fields: Vec<FieldCtx> = msg
        .fields
        .iter()
        .enumerate()
        .map(|(i, f)| FieldCtx {
            name: f.name.clone(),
            id: f.id.unwrap_or(i as u32),
            rust_type: hcl_type_to_rust(&f.field_type).unwrap_or_else(|e| {
                eprintln!("warning: {e}, falling back to String");
                "String".to_string()
            }),
            cpp_type: hcl_type_to_cpp(&f.field_type).unwrap_or_else(|e| {
                eprintln!("warning: {e}, falling back to std::string");
                "std::string".to_string()
            }),
            description: f.description.clone().unwrap_or_default(),
            unit: f.unit.clone().unwrap_or_default(),
            index: i,
        })
        .collect();

    Ok(MessageCtx {
        name: to_pascal(name),
        id: fnv1a_32(name),
        fields,
    })
}

// ── Type mapping ─────────────────────────────────────────────────────────

fn hcl_type_to_rust(hcl: &str) -> anyhow::Result<String> {
    if let Some(inner) = hcl.strip_suffix("[]") {
        return Ok(format!("Vec<{}>", hcl_type_to_rust(inner)?));
    }

    let rust = match hcl {
        "uint32" => "u32",
        "uint64" => "u64",
        "int32" => "i32",
        "int64" => "i64",
        "float" | "double" => "f32",
        "string" => "String",
        "bool" => "bool",
        "enum" => "i32",
        "bytes" => "Vec<u8>",
        other => anyhow::bail!("unknown HCL type: {other}"),
    };
    Ok(rust.to_string())
}

fn hcl_type_to_cpp(hcl: &str) -> anyhow::Result<String> {
    if let Some(inner) = hcl.strip_suffix("[]") {
        return Ok(format!("std::vector<{}>", hcl_type_to_cpp(inner)?));
    }

    let cpp = match hcl {
        "uint32" => "uint32_t",
        "uint64" => "uint64_t",
        "int32" => "int32_t",
        "int64" => "int64_t",
        "float" | "double" => "float",
        "string" => "std::string",
        "bool" => "bool",
        "enum" => "int32_t",
        "bytes" => "std::vector<uint8_t>",
        other => anyhow::bail!("unknown HCL type: {other}"),
    };
    Ok(cpp.to_string())
}

// ── Name conversion ──────────────────────────────────────────────────────

fn to_pascal(s: &str) -> String {
    let mut out = String::with_capacity(s.len());
    let mut upper = true;
    for ch in s.chars() {
        if ch == '_' {
            upper = true;
        } else if upper {
            out.extend(ch.to_uppercase());
            upper = false;
        } else {
            out.push(ch);
        }
    }
    out
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
