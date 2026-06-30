//! Code generator: reads an HCL robot config and emits code via a Tera template.
//!
//! Templates live in `hcl/*.tera`.  Use `-t` to pick one:
//!   cargo run --bin hcl_codegen -- -t rust.tera -o src/messages.rs
//!   cargo run --bin hcl_codegen -- -t cpp.tera  -o src/messages.h

use anyhow::Context;
use clap::Parser;
use coder::{load_robot_config, DeviceConfig, MessageConfig, RobotConfig};
use serde::Serialize;
use tera::{Context as TeraContext, Tera};

fn fnv1a_32(s: &str) -> u32 {
    // 1. Initialize the hash with the 32-bit offset basis
    let mut hash: u32 = 2166136261;
    
    // 2. Process each byte of the string
    for byte in s.as_bytes() {
        // XOR the lower 8 bits of the hash with the byte
        hash ^= *byte as u32;
        // Multiply by the FNV prime (wrapping_mul prevents overflow panics in debug mode)
        hash = hash.wrapping_mul(16777619);
    }
    
    hash
}

#[derive(Parser)]
#[command(about = "Generate code from an HCL robot config")]
struct Args {
    /// Path to the HCL robot config
    #[arg(short = 'i', long, default_value = "hcl/robot.hcl")]
    input: String,
    /// Output file
    #[arg(short = 'o', long, default_value = "src/generated.rs")]
    output: String,
    /// Tera template name (from hcl/*.tera)
    #[arg(short = 't', long, default_value = "rust.tera")]
    template: String,
}

fn main() -> anyhow::Result<()> {
    let args = Args::parse();
    let cfg = load_robot_config(&args.input)
        .with_context(|| format!("failed to load {}", args.input))?;

    let ctx = build_template_context(&cfg)?;
    let tera = Tera::new("hcl/*.tera")
        .with_context(|| "failed to load templates from hcl/*.tera")?;
    let code = tera
        .render(&args.template, &ctx)
        .with_context(|| format!("template rendering failed for {}", args.template))?;

    std::fs::write(&args.output, &code)
        .with_context(|| format!("failed to write {}", args.output))?;

    let msg_count = ctx.get("messages").and_then(|v| v.as_array()).map(|a| a.len()).unwrap_or(0);
    let dev_count = ctx.get("devices").and_then(|v| v.as_array()).map(|a| a.len()).unwrap_or(0);
    eprintln!(
        "Wrote {} messages, {} devices to {}",
        msg_count, dev_count, args.output
    );
    Ok(())
}

// ── Template data model ─────────────────────────────────────────────────



#[derive(Serialize)]
struct TemplateCtx {
    robot: RobotCtx,
    messages: Vec<MessageCtx>,
    devices: Vec<DeviceCtx>,
}

#[derive(Serialize)]
struct RobotCtx {
    name: String,
    id: u32,
    multicast_port: u16,
}

#[derive(Serialize)]
struct DeviceCtx {
    name: String,
    id: u32,
    description: String,
    mac: String,
    mdns: String,
    endpoints: Vec<EndpointCtx>,
}

#[derive(Serialize)]
struct EndpointCtx {
    name: String,
    interface_ref: String,
    description: String,
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
    rust_type: String,
    cpp_type: String,
    description: String,
    index: usize,
}

fn build_template_context(cfg: &RobotConfig) -> anyhow::Result<TeraContext> {
    let mut messages: Vec<MessageCtx> = Vec::new();
    let mut devices: Vec<DeviceCtx> = Vec::new();

    let mut iface_names: Vec<&String> = cfg.interfaces.keys().collect();
    iface_names.sort();
    for iface_name in iface_names {
        let iface = &cfg.interfaces[iface_name];
        let mut msg_names: Vec<&String> = iface.messages.keys().collect();
        msg_names.sort();
        for msg_name in msg_names {
            messages.push(message_to_ctx(msg_name, &iface.messages[msg_name])?);
        }
    }

    let mut dev_names: Vec<&String> = cfg.devices.keys().collect();
    dev_names.sort();
    for dev_name in dev_names {
        devices.push(device_to_ctx(dev_name, &cfg.devices[dev_name]));
    }

    let template_ctx = TemplateCtx {
        robot: RobotCtx {
            name: cfg.name.clone(),
            id: fnv1a_32(&cfg.name),
            multicast_port: cfg.multicast_port,
        },
        messages,
        devices,
    };
    let ctx = TeraContext::from_serialize(&template_ctx)?;
    Ok(ctx)
}

fn device_to_ctx(name: &str, dev: &DeviceConfig) -> DeviceCtx {
    let mut ep_names: Vec<&String> = dev.endpoints.keys().collect();
    ep_names.sort();
    let endpoints: Vec<EndpointCtx> = ep_names
        .into_iter()
        .map(|ep_name| {
            let ep = &dev.endpoints[ep_name];
            EndpointCtx {
                name: ep_name.clone(),
                interface_ref: ep.interface_ref.clone(),
                description: ep.description.clone().unwrap_or_default(),
            }
        })
        .collect();

    DeviceCtx {
        name: name.to_string(),
        id: fnv1a_32(name),
        description: dev.description.clone().unwrap_or_default(),
        mac: dev.mac.clone().unwrap_or_default(),
        mdns: dev.mdns.clone().unwrap_or_default(),
        endpoints,
    }
}

fn message_to_ctx(name: &str, msg: &MessageConfig) -> anyhow::Result<MessageCtx> {
    let fields: Vec<FieldCtx> = msg
        .fields
        .iter()
        .enumerate()
        .map(|(i, f)| FieldCtx {
            name: f.name.clone(),
            rust_type: hcl_type_to_rust(&f.field_type)
                .unwrap_or_else(|e| {
                    eprintln!("warning: {e}, falling back to String");
                    "String".to_string()
                }),
            cpp_type: hcl_type_to_cpp(&f.field_type)
                .unwrap_or_else(|e| {
                    eprintln!("warning: {e}, falling back to std::string");
                    "std::string".to_string()
                }),
            description: f.description.clone().unwrap_or_default(),
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
    let rust = match hcl {
        "uint32" => "u32",
        "uint64" => "u64",
        "int32" => "i32",
        "int64" => "i64",
        "float" | "double" => "f32",
        "string" => "String",
        "bool" => "bool",
        "enum" => "i32",
        other => anyhow::bail!("unknown HCL type: {other}"),
    };
    Ok(rust.to_string())
}

fn hcl_type_to_cpp(hcl: &str) -> anyhow::Result<String> {
    let cpp = match hcl {
        "uint32" => "uint32_t",
        "uint64" => "uint64_t",
        "int32" => "int32_t",
        "int64" => "int64_t",
        "float" | "double" => "float",
        "string" => "std::string",
        "bool" => "bool",
        "enum" => "int32_t",
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
