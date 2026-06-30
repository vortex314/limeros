use coder::load_robot_config;

fn main() -> anyhow::Result<()> {
    let cfg = load_robot_config("hcl/robot.hcl")?;
    println!("Robot: {} (model: {})", cfg.name, cfg.model);

    // Resolve endpoint → interface references.
    for (dev_name, dev) in &cfg.devices {
        for (ep_name, ep) in &dev.endpoints {
            let iface = cfg.resolve_endpoint_interface(dev_name, ep_name);
            println!(
                "  {dev_name}/{ep_name} -> {}",
                iface.map(|i| i.description.as_deref().unwrap_or("?")).unwrap_or("UNRESOLVED")
            );
        }
    }

    // Fields preserve declaration order.
    if let Some(iface) = cfg.interfaces.get("HoverboardDrive") {
        if let Some(msg) = iface.messages.get("HoverboardEvent") {
            println!("\nHoverboardEvent: {} fields in order", msg.fields.len());
            for f in msg.fields.iter().take(5) {
                println!("  {}: {}", f.name, f.field_type);
            }
        }
    }

    Ok(())
}
