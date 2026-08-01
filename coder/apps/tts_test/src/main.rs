//! tts_test — simple test app for Text-to-Speech via speech-dispatcher.
//!
//! Uses the spd-say command-line tool (part of speech-dispatcher).
//!
//! Usage: cargo run --bin tts_test [-- "text to speak"]

use std::process::Command;

fn main() {
    let text: String = std::env::args().skip(1).collect::<Vec<_>>().join(" ");
    let text = if text.is_empty() {
        "Hello, this is a test of the text to speech system."
    } else {
        &text
    };

    println!("TTS: \"{text}\"");

    let status = Command::new("spd-say")
        .args(["-o", "espeak-ng", "-w"])
        .arg(text)
        .status()
        .expect("failed to run spd-say — is speech-dispatcher installed?");

    if !status.success() {
        eprintln!("spd-say exited with error: {status}");
        std::process::exit(1);
    }
    println!("Done.");
}
