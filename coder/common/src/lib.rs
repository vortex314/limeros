pub mod base_message;
pub mod config;
pub mod endpoint;
pub mod fnv;
pub mod logger;
pub mod node;

pub use config::load_robot_config;
pub use config::{EndpointConfig, MessageConfig, RobotConfig, SubscribeConfig};
pub use fnv::fnv1a_32;
use generated::generated::{Envelope, opt_id_to_string};
pub use logger::init;
pub use endpoint::check_envelope;


pub fn envelope_to_string(envelope: &Envelope) -> String {
    format!(
        "Envelope {{ src: {:?}, dst: {:?}, msg_type: {:?} }}",
        opt_id_to_string(envelope.src),
        opt_id_to_string(envelope.dst),
        opt_id_to_string(envelope.msg_type) 
    )
}
