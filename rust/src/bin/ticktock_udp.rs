use std::env;
use std::net::UdpSocket;
use std::thread;
use std::time::{Duration, SystemTime, UNIX_EPOCH};

fn unix_timestamp_secs() -> u64 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .expect("system clock is before UNIX_EPOCH")
        .as_secs()
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let addr = args
        .get(1)
        .cloned()
        .unwrap_or_else(|| "127.0.0.1:6181".to_string());
    let metric = args
        .get(2)
        .cloned()
        .unwrap_or_else(|| "limeros.heartbeat".to_string());
    let host_tag = args
        .get(3)
        .cloned()
        .unwrap_or_else(|| "localhost".to_string());

    let socket = UdpSocket::bind("0.0.0.0:0").expect("failed to bind local UDP socket");

    println!(
        "Sending OpenTSDB metrics to {addr}: metric={metric}, host={host_tag} (once per second)"
    );

    let mut value: u64 = 1;
    loop {
        let ts = unix_timestamp_secs();
        let line = format!("{metric} {ts} {value} host={host_tag} source=limeros\n");
        println!("Sending: {line}");

        match socket.send_to(line.as_bytes(), &addr) {
            Ok(_) => println!("sent: {}", line.trim_end()),
            Err(e) => eprintln!("send failed: {e}"),
        }

        value += 1;
        thread::sleep(Duration::from_secs(1));
    }
}
