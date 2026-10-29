use generated::generated::MotorEvent;


struct Motor {
    enabled: bool,  
    rpm_target: i32,
    rpm_actual: i32,
}

impl Motor {
    pub fn handle_envelope(&mut self, envelope: &generated::generated::Envelope) -> anyhow::Result<()> {
        match envelope.msg_type.unwrap() {
            generated::generated::MotorEvent::MSG_ID => {
                let motor_event = MotorEvent::from_bytes(envelope.payload.as_ref().unwrap())?;
                // Handle motor event
            }
            any => {
                eprintln!("Unhandled message type: {}", any);
            }
        }
        Ok(())
    }
}