use std::fs;
use std::io::Write;

fn main() -> std::io::Result<()> {
    // Read robot.hcl
    let hcl_content = fs::read_to_string("robot.hcl")?;

    // Start XML
    let mut xml = String::from(
        r#"<?xml version="1.0"?>
<mavlink>
  <messages>
"#,
    );

    // Parse messages (very simple parser, assumes format: message_name { type field ... })
    for block in hcl_content.split("\n\n") {
        if let Some((name, fields)) = block.split_once('{') {
            let msg_name = name.trim();
            xml.push_str(&format!(r#"    <message name="{}" id="0">"#, msg_name));
            for line in fields.lines() {
                let line = line.trim().trim_end_matches('}');
                if line.is_empty() { continue; }
                let mut parts = line.split_whitespace();
                let field_type = parts.next().unwrap_or("");
                let field_name = parts.next().unwrap_or("");
                let xml_type = match field_type {
                    "int" => "int32_t",
                    "float" => "float",
                    "string" => "char[50]",
                    _ => "uint8_t",
                };
                xml.push_str(&format!(
                    r#"
      <field type="{}" name="{}"/>"#,
                    xml_type, field_name
                ));
            }
            xml.push_str("\n    </message>\n");
        }
    }

    xml.push_str("  </messages>\n</mavlink>\n");

    // Write to robot.xml
    let mut file = fs::File::create("robot.xml")?;
    file.write_all(xml.as_bytes())?;

    Ok(())
}