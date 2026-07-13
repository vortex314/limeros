//! Robot configuration loader for HCL files.
//!
//! Parses HCL robot configuration files (like `hcl/robot.hcl`) into typed Rust
//! structures using the [`hcl`] crate's AST (via the `hcl-rs` package).
//!
//! # Example
//!
//! ```no_run
//! use coder::load_robot_config;
//!
//! let cfg = load_robot_config("hcl/robot.hcl").unwrap();
//! println!("Robot: {} (model {})", cfg.name, cfg.model);
//! for (name, ep) in &cfg.endpoints {
//!     println!("  endpoint {name}: {:?}", ep.services);
//! }
//! ```

use hcl::template::{Element, Template};
use hcl::{Block, Body, Expression, TemplateExpr, TraversalOperator};
use log::info;
use regex::Regex;
use std::collections::HashMap;
use std::fs;
use std::path::Path;
use std::str::FromStr;

use crate::fnv1a_32;

// ── Public types ──────────────────────────────────────────────────────────

/// Complete robot configuration parsed from an HCL file.
#[derive(Debug, Clone)]
pub struct RobotConfig {
    /// The robot's label (e.g. `"ronald"`).
    pub name: String,
    /// Model string (`model = "Limero-v1"`).
    pub model: String,
    /// Multicast port for the robot.
    pub multicast_port: u16,
    /// Broker port for the robot.
    pub broker_port: u16,
    /// Multicast address for the robot.
    pub multicast_addr: Option<String>,
    /// Optional human-readable description.
    pub description: Option<String>,
    /// Named endpoints, keyed by label.
    pub endpoints: HashMap<String, EndpointConfig>,
    /// Named messages, keyed by label.
    pub messages: HashMap<String, MessageConfig>,
    pub id_map: HashMap<u32, String>,
}

impl RobotConfig {
    pub fn load_from_file(path: impl AsRef<Path>) -> anyhow::Result<Self> {
        load_robot_config(path)
    }



    pub fn id_to_name(&self, id: Option<u32>) -> String {
        match id {
            Some(id) => self.id_map.get(&id).cloned().unwrap_or_else(|| id.to_string()),
            None => "None".to_string(),
        }
    }
}

/// An endpoint on a device, referencing an interface.
#[derive(Debug, Clone)]
pub struct EndpointConfig {
    /// Dotted-path references to interfaces (e.g. `"interface.System"`).
    pub interfaces: Vec<String>,
    pub services: Vec<String>,
    pub events: Vec<String>,
    pub replies: Vec<String>,
    pub subscribes: Vec<SubscribeConfig>,
    pub description: Option<String>,
}

#[derive(Debug, Clone)]
pub struct SubscribeConfig {
    pub src: Option<String>,
    pub msg_type: Option<String>,
    pub dst: Option<String>,
}

/// A message type with named fields, kept in declaration order.
#[derive(Debug, Clone)]
pub struct MessageConfig {
    pub description: Option<String>,
    /// Fields in declaration order.
    pub fields: Vec<FieldConfig>,
}

/// A single field inside a message.
#[derive(Debug, Clone)]
pub struct FieldConfig {
    /// Field name (the HCL block label).
    pub name: String,
    /// Optional explicit field id used for map-style serialization.
    pub id: Option<u32>,
    /// HCL type string (e.g. `"uint32"`, `"float"`, `"string"`).
    pub field_type: String,
    pub description: Option<String>,
    pub unit: Option<String>,
}

// ── Public entry points ───────────────────────────────────────────────────

/// Load a robot configuration from an HCL file on disk.
pub fn load_robot_config(path: impl AsRef<Path>) -> anyhow::Result<RobotConfig> {
    let content = fs::read_to_string(path)?;
    // Normalise non-standard `$ { expr }` / `${ expr }` syntax into a plain
    // traversal expression that HCL can parse.
    let content = normalize_template_refs(&content);
    let body = hcl::parse(&content)?;
    parse_robot_body(&body)
}

/// Normalise non-standard HCL syntax used in `robot.hcl`:
///
/// 1. Strip `$ { … }` / `${ … }` wrappers around bare traversal references.
/// 2. Expand single-line blocks with multiple attributes into multi-line form
///    (standard HCL requires each attribute on its own line inside a block body).
fn normalize_template_refs(input: &str) -> String {
    // Step 1: Remove `$ {` / `${` … `}` wrappers.
    let re_dollar = Regex::new(r#"\$\s*\{\s*([^}]+)\s*\}"#).unwrap();
    let s = re_dollar.replace_all(input, "$1").to_string();

    // Step 1b: Normalize inline object entries inside lists, e.g.
    //   subscribes = [ { src="a" msg_type="X" dst=null } ]
    // into a comma-delimited object so HCL parses it as an object literal.
    let re_attr = Regex::new(r#"(\w+)\s*=\s*("[^"]*"|[^\s",}]+)"#).unwrap();
    let re_list_obj = Regex::new(r#"\[\s*\{\s*([^{}\n]+)\s*\}\s*\]"#).unwrap();
    let s = re_list_obj
        .replace_all(&s, |caps: &regex::Captures| {
            let body = caps.get(1).map(|m| m.as_str()).unwrap_or_default();
            let attrs: Vec<String> = re_attr
                .captures_iter(body)
                .map(|c| format!("{} = {}", &c[1], &c[2]))
                .collect();
            if attrs.is_empty() {
                caps.get(0).map(|m| m.as_str()).unwrap_or_default().to_string()
            } else {
                format!("[ {{ {} }} ]", attrs.join(", "))
            }
        })
        .to_string();

    // Step 2: Expand inline multi-attribute blocks.
    //   field "x" { type = "u32" description = "..." }
    //   field "x" {
    //     type = "u32"
    //     description = "..."
    //   }
    // Strategy: for lines that contain `{` … `}` with more than one `=`, split
    // the body content at `= "..."` / `= number` / `= bool` boundaries.
    let re_inline = Regex::new(
        r#"^(\s*)(\w+\s+"[^"]+"\s*\{)\s*(.*?)\s*(\})\s*$"#
    ).unwrap();
    let re_attr = Regex::new(
        r#"(\w+)\s*=\s*("[^"]*"|\$\{?[^}]*\}?|[^\s"]+)"#
    ).unwrap();

    let mut lines: Vec<String> = Vec::new();
    for line in s.lines() {
        if let Some(caps) = re_inline.captures(line) {
            let indent = &caps[1];
            let header = &caps[2];
            let body = &caps[3];
            lines.push(format!("{indent}{header}"));
            for attr in re_attr.captures_iter(body) {
                let key = &attr[1];
                let val = &attr[2];
                lines.push(format!("{indent}  {key} = {val}"));
            }
            lines.push(format!("{indent}}}"));
        } else {
            lines.push(line.to_string());
        }
    }
    lines.join("\n")
}

/// Parse a robot configuration from an already-parsed [`hcl::Body`].
pub fn parse_robot_body(body: &Body) -> anyhow::Result<RobotConfig> {
    let robot_block = body
        .blocks()
        .find(|b| &*b.identifier() == "robot")
        .ok_or_else(|| anyhow::anyhow!("no 'robot' block found"))?;

    let name = robot_block
        .labels()
        .first()
        .map(|l| l.as_str().to_string())
        .unwrap_or_default();

    let inner = robot_block.body();

    let messages = parse_labeled_blocks(inner, "message", parse_message)?;
    let endpoints = parse_labeled_blocks(inner, "endpoint", parse_endpoint)?;

    let mut id_map: HashMap<u32, String> = messages
        .keys()
        .map(|msg_name| {
            let id = fnv1a_32(msg_name);
            (id, msg_name.clone())
        })
        .collect();
    for (ep_name, _) in &endpoints {
        let id = fnv1a_32(ep_name);
        id_map.insert(id, ep_name.clone());
    }

    // list all known names
    id_map.iter().for_each(|(id, name)| {
        info!("Known endpoint: {} -> {}", id, name);
    });

    Ok(RobotConfig {
        model: get_attr_string(inner, "model")?,
        description: get_attr_optional(inner, "description"),
        endpoints,
        messages,
        name,
        multicast_port: get_attr_optional(inner, "multicast_port")
            .and_then(|s| s.parse::<u16>().ok())
            .unwrap_or(0),
        broker_port: get_attr_optional(inner, "broker_port")
            .and_then(|s| s.parse::<u16>().ok())
            .unwrap_or(0),
        multicast_addr: get_attr_optional(inner, "multicast_addr"),
        id_map,
    })
}

// ── Block parsers ─────────────────────────────────────────────────────────

fn parse_endpoint(block: &Block) -> anyhow::Result<(String, EndpointConfig)> {
    let name = label(block);
    let body = block.body();
    Ok((
        name,
        EndpointConfig {
            interfaces: get_attr_list_strings(body, "interfaces"),
            services: get_attr_list_strings(body, "services"),
            events: get_attr_list_strings(body, "events"),
            replies: get_attr_list_strings(body, "replies"),
            subscribes: get_attr_subscribes(body, "subscribes"),
            description: get_attr_optional(body, "description"),
        },
    ))
}

fn parse_message(block: &Block) -> anyhow::Result<(String, MessageConfig)> {
    let name = label(block);
    let body = block.body();
    let mut fields = Vec::new();
    for b in body.blocks().filter(|b| &*b.identifier() == "field") {
        fields.push(parse_field(b)?);
    }
    Ok((
        name,
        MessageConfig {
            description: get_attr_optional(body, "description"),
            fields,
        },
    ))
}

fn parse_field(block: &Block) -> anyhow::Result<FieldConfig> {
    let name = label(block);
    let body = block.body();
    Ok(FieldConfig {
        name,
        id: get_attr_u32_optional(body, "id"),
        field_type: get_attr_string(body, "type")?,
        description: get_attr_optional(body, "description"),
        unit: get_attr_optional(body, "unit"),
    })
}

// ── Generic helpers ───────────────────────────────────────────────────────

/// Convenience: first label of a block as a String.
fn label(block: &Block) -> String {
    block
        .labels()
        .first()
        .map(|l| l.as_str().to_string())
        .unwrap_or_default()
}

/// Parse all labeled blocks of a given `block_type` into a `HashMap`.
fn parse_labeled_blocks<T>(
    body: &Body,
    block_type: &str,
    parser: fn(&Block) -> anyhow::Result<(String, T)>,
) -> anyhow::Result<HashMap<String, T>> {
    let mut map = HashMap::new();
    for block in body
        .blocks()
        .filter(|b| &*b.identifier() == block_type)
    {
        let (name, config) = parser(block)?;
        map.insert(name, config);
    }
    Ok(map)
}

/// Read an optional attribute as a string.
fn get_attr_optional(body: &Body, key: &str) -> Option<String> {
    body.attributes()
        .find(|a| &*a.key == key)
        .and_then(|a| expr_to_string(&a.expr))
}

/// Read a required attribute as a string.
fn get_attr_string(body: &Body, key: &str) -> anyhow::Result<String> {
    body.attributes()
        .find(|a| &*a.key == key)
        .and_then(|a| expr_to_string(&a.expr))
        .ok_or_else(|| anyhow::anyhow!("missing attribute '{key}'"))
}

/// Read an optional attribute as a u32.
fn get_attr_u32_optional(body: &Body, key: &str) -> Option<u32> {
    body.attributes()
        .find(|a| &*a.key == key)
        .and_then(|a| expr_to_string(&a.expr))
        .and_then(|s| s.parse::<u32>().ok())
}

/// Read an optional attribute as a list of strings.
fn get_attr_list_strings(body: &Body, key: &str) -> Vec<String> {
    match body.attributes().find(|a| &*a.key == key).map(|a| &a.expr) {
        Some(Expression::Array(items)) => items.iter().filter_map(expr_to_string).collect(),
        Some(expr) => expr_to_string(expr).map(|s| vec![s]).unwrap_or_default(),
        None => Vec::new(),
    }
}

fn get_attr_subscribes(body: &Body, key: &str) -> Vec<SubscribeConfig> {
    let Some(expr) = body.attributes().find(|a| &*a.key == key).map(|a| &a.expr) else {
        return Vec::new();
    };

    let Expression::Array(items) = expr else {
        return Vec::new();
    };

    items
        .iter()
        .filter_map(|item| {
            let Expression::Object(obj) = item else {
                return None;
            };
            let mut src: Option<String> = None;
            let mut msg_type: Option<String> = None;
            let mut dst: Option<String> = None;

            for (k, v) in obj.iter() {
                let value = expr_to_string(v);
                match k.to_string().as_str() {
                    "src" => src = value.filter(|s| s != "null"),
                    "msg_type" => msg_type = value.filter(|s| s != "null"),
                    "dst" => dst = value.filter(|s| s != "null"),
                    _ => {}
                }
            }

            Some(SubscribeConfig { src, msg_type, dst })
        })
        .collect()
}

/// Convert an HCL [`Expression`] into a plain Rust `String`.
fn expr_to_string(expr: &Expression) -> Option<String> {
    match expr {
        Expression::String(s) => Some(s.clone()),
        Expression::Number(n) => Some(n.to_string()),
        Expression::Bool(b) => Some(b.to_string()),
        Expression::Null => Some("null".to_string()),
        Expression::Variable(v) => Some(v.as_str().to_string()),
        Expression::TemplateExpr(tmpl) => {
            let raw = match tmpl.as_ref() {
                TemplateExpr::QuotedString(s) => s,
                TemplateExpr::Heredoc(h) => &h.template,
            };
            // Try parsing as a template to extract interpolations/traversals.
            extract_template_value(raw)
        }
        Expression::Traversal(trav) => {
            let mut parts: Vec<String> = Vec::new();
            // Include the root expression of the traversal.
            if let Some(root) = expr_to_string(&trav.expr) {
                parts.push(root);
            }
            for op in &trav.operators {
                match op {
                    TraversalOperator::GetAttr(ident) => {
                        parts.push(ident.as_str().to_string());
                    }
                    _ => {}
                }
            }
            if parts.is_empty() {
                Some(expr.to_string())
            } else {
                Some(parts.join("."))
            }
        }
        _ => Some(expr.to_string()),
    }
}

/// Given the raw content of a template expression (e.g. `"${interface.HoverboardDrive}"`),
/// extract the meaningful value: either the literal string (if no interpolation) or
/// the dotted traversal path inside the interpolation.
fn extract_template_value(raw: &str) -> Option<String> {
    // Use hcl's Template parser to decompose the template string.
    // raw looks like "${interface.HoverboardDrive}" or "plain text".
    let template = Template::from_str(raw).ok()?;
    let mut parts: Vec<String> = Vec::new();

    for element in template.elements() {
        match element {
            Element::Literal(lit) => parts.push(lit.clone()),
            Element::Interpolation(interp) => {
                if let Some(inner) = expr_to_string(&interp.expr) {
                    parts.push(inner);
                }
            }
            Element::Directive(_) => {}
        }
    }

    if parts.is_empty() {
        None
    } else {
        Some(parts.join(""))
    }
}
