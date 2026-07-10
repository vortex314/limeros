use anyhow::{Context, Result};
use clap::Parser;
use common::base_message::{EndpointAnnounce, Msg, UdpMessage};
use common::endpoint::Endpoint;
use common::fnv::fnv1a_32;
use common::load_robot_config;
use common::logger;
use common::node::UdpNode;
use generated::generated;
use log::info;
use std::collections::HashMap;
use std::io;
use std::net::Ipv4Addr;
use std::net::SocketAddr;
use std::sync::{Arc, Mutex};
use std::time::Duration;

use crossterm::{
    event::{self, DisableMouseCapture, EnableMouseCapture, Event, KeyCode},
    execute,
    terminal::{disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen},
};
use ratatui::{
    layout::{Constraint, Layout},
    style::{Color, Modifier, Style},
    widgets::{Block, Borders, Cell, Paragraph, Row, Table},
    Frame, Terminal,
};
use ratatui::backend::CrosstermBackend;

// ── CLI ──────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug, Clone)]
#[command(about = "TUI message bus sniffer")]
struct Args {
    #[arg(long, default_value = "tui_sniffer")]
    endpoint: String,

    #[arg(long, default_value = generated::MULTICAST_ADDR)]
    group: String,

    #[arg(long)]
    port: Option<u16>,

    #[arg(long, default_value = "0.0.0.0")]
    bind: String,

    #[arg(short = 'c', long, default_value = "hcl/robot.hcl")]
    config: String,
}

// ── Log entry ────────────────────────────────────────────────────────────────

#[derive(Clone)]
struct LogEntry {
    timestamp: String,   // "HH:MM:SS.mmm"
    src: String,
    dst: String,
    msg_type: String,
    req_id: String,
    payload: String,
}

// ── App state ────────────────────────────────────────────────────────────────

struct AppState {
    entries: Vec<LogEntry>,
    name_map: HashMap<u32, String>,
    scroll: usize,
    follow_bottom: bool,
    message_count: u64,
    endpoint_name: String,
    endpoint_id: u32,
    connected: bool,
}

impl AppState {
    fn new() -> Self {
        AppState {
            entries: Vec::new(),
            name_map: HashMap::new(),
            scroll: 0,
            follow_bottom: true,
            message_count: 0,
            endpoint_name: String::new(),
            endpoint_id: 0,
            connected: false,
        }
    }
}

// ── Helpers ──────────────────────────────────────────────────────────────────

/// Deserialize a payload into a known message struct (Debug display),
/// falling back to CBOR diagnostic notation if the type is unknown or
/// deserialization fails.
fn format_payload_typed(msg_type_id: u32, payload: Option<&[u8]>) -> String {
    use common::base_message::{EndpointAnnounce, EndpointAnnounceReply, UdpMessage};

    let bytes = match payload {
        Some(b) => b,
        None => return "-".into(),
    };

    // Try each known message type.
    // We match on the numeric ID so only one deserialization is attempted.
    macro_rules! try_deser {
        ($ty:ty) => {{
            if msg_type_id == <$ty>::id() {
                if let Ok(m) = <$ty>::from_bytes(bytes) {
                    return format!("{:?}", m);
                }
            }
        }};
    }

    try_deser!(EndpointAnnounce);
    try_deser!(EndpointAnnounceReply);
    try_deser!(UdpMessage);
    try_deser!(generated::BrokerSubscribeRequest);
    try_deser!(generated::HoverboardEvent);
    try_deser!(generated::HoverboardReply);
    try_deser!(generated::HoverboardRequest);
    try_deser!(generated::PingReply);
    try_deser!(generated::PingRequest);
    try_deser!(generated::Ps4Event);
    try_deser!(generated::Ps4Request);
    try_deser!(generated::SysEvent);
    try_deser!(generated::SysReply);
    try_deser!(generated::SysRequest);

    // Fallback: generic CBOR diagnostic
    cbor2::from_slice::<cbor2::Value>(bytes)
        .map(|v| format!("{:?}", v))
        .unwrap_or_else(|_| format!("<{}b>", bytes.len()))
}

fn msg_type_name(id: u32) -> &'static str {
    match id {
        3190208493 => "BrokerSubscribeReq",
        2371693343 => "EndpointAnnounce",
        3238220441 => "EndpointAnnReply",
        1293877827 => "UdpMessage",
        104988481 => "HoverboardEvent",
        2095960949 => "HoverboardReply",
        2735870956 => "HoverboardRequest",
        1594103907 => "PingReply",
        31253678 => "PingRequest",
        4282593576 => "Ps4Event",
        1992038561 => "Ps4Request",
        924742914 => "SysEvent",
        2952492394 => "SysReply",
        2966412411 => "SysRequest",
        _ => "?",
    }
}

fn fmt_endpoint(name_map: &HashMap<u32, String>, id: Option<u32>) -> String {
    match id {
        Some(v) => name_map.get(&v).cloned().unwrap_or_else(|| v.to_string()),
        None => "-".into(),
    }
}

fn build_name_map(config_path: &str) -> HashMap<u32, String> {
    let mut map = HashMap::new();
    if let Ok(cfg) = load_robot_config(config_path) {
        for (dev_name, dev) in &cfg.devices {
            for ep_name in dev.endpoints.keys() {
                // and the bare name ID ("endpoint") used by apps like pinger.
                map.insert(fnv1a_32(&ep_name), ep_name.clone());
            }
        }
    }
    map
}

fn process_message(
    msg: &UdpMessage,
    name_map: &mut HashMap<u32, String>,
    count: &mut u64,
) -> LogEntry {
    // Collect endpoint names from announces
    if msg.msg_type == Some(EndpointAnnounce::id()) {
        if let Some(payload) = &msg.payload {
            if let Ok(announce) = EndpointAnnounce::from_bytes(payload) {
                if let (Some(ep_id), Some(ep_name)) = (announce.endpoint_id, announce.endpoint_name)
                {
                    name_map.entry(ep_id).or_insert_with(|| {
                        info!("Discovered endpoint: {} (id={})", ep_name, ep_id);
                        ep_name
                    });
                }
            }
        }
    }

    *count += 1;

    LogEntry {
        timestamp: chrono::Local::now().format("%H:%M:%S%.3f").to_string(),
        src: fmt_endpoint(name_map, msg.src),
        dst: fmt_endpoint(name_map, msg.dst),
        msg_type: msg_type_name(msg.msg_type.unwrap_or(0)).to_string(),
        req_id: msg.req_id.map(|v| v.to_string()).unwrap_or_default(),
        payload: format_payload_typed(msg.msg_type.unwrap_or(0), msg.payload.as_deref()),
    }
}

// ── Network task ─────────────────────────────────────────────────────────────

async fn run_network(
    state: Arc<Mutex<AppState>>,
    entry_tx: std::sync::mpsc::Sender<LogEntry>,
    endpoint_name: String,
    group: Ipv4Addr,
    port: u16,
    _config_path: String,
) -> Result<()> {
    let endpoint_id = fnv1a_32(&endpoint_name);
    let node = UdpNode::new(endpoint_id, SocketAddr::new(group.into(), port));
    let mut endpoint = Endpoint::new_from_name(&endpoint_name, node);

    endpoint.bind().await?;
    info!("Discovering broker...");
    endpoint.broker_handshake().await?;
    info!("Broker found, subscribing to all messages");

    let sub_req = generated::BrokerSubscribeRequest {
        src: None,
        msg_type: None,
    };
    endpoint
        .send(UdpMessage {
            src: Some(endpoint_id),
            dst: None,
            msg_type: Some(generated::BrokerSubscribeRequest::id()),
            req_id: None,
            payload: Some(sub_req.to_bytes()?),
        })
        .await?;

    // Mark as connected
    {
        let mut app = state.lock().unwrap();
        app.connected = true;
    }
    info!("Connected and subscribed");

    let endpoint = Arc::new(endpoint);
    let ep_clone = endpoint.clone();

    // Receive loop
    loop {
        let (msg, _addr) = ep_clone.receive().await?;

        let entry = {
            let mut app = state.lock().unwrap();
            // Use destructuring to get simultaneous mutable borrows
            let AppState {
                ref mut name_map,
                ref mut message_count,
                ..
            } = *app;
            process_message(&msg, name_map, message_count)
        };

        if entry_tx.send(entry).is_err() {
            break; // TUI closed
        }
    }

    Ok(())
}

// ── TUI render ───────────────────────────────────────────────────────────────

fn render_ui(f: &mut Frame, app: &AppState) {
    let area = f.area();

    let chunks = Layout::vertical([
        Constraint::Length(1),  // title bar
        Constraint::Min(1),     // table
        Constraint::Length(1),  // help bar
    ])
    .split(area);

    // Title bar
    let status = if app.connected { "connected" } else { "connecting..." };
    let title = format!(
        " tui_sniffer | {} messages | broker: {} | {} (0x{:08x}) ",
        app.message_count,
        status,
        app.endpoint_name,
        app.endpoint_id,
    );
    let title_widget = Paragraph::new(title)
        .style(Style::default().bg(Color::DarkGray).fg(Color::White));
    f.render_widget(title_widget, chunks[0]);

    // Table
    let visible_rows = chunks[1].height.saturating_sub(3) as usize; // header + borders
    let total = app.entries.len();
    let scroll = if app.follow_bottom && total > visible_rows {
        total.saturating_sub(visible_rows)
    } else {
        app.scroll.min(total.saturating_sub(visible_rows.min(total)))
    };

    let start = scroll;
    let end = (start + visible_rows).min(total);
    let slice = &app.entries[start..end];

    let header = Row::new(["  #", "Time", "Src", "Dst", "Type", "ReqID", "Payload"])
        .style(Style::default().fg(Color::Yellow).add_modifier(Modifier::BOLD))
        .height(1);

    let rows: Vec<Row> = slice
        .iter()
        .enumerate()
        .map(|(i, e)| {
            let num = start + i + 1;
            let style = if i % 2 == 0 {
                Style::default()
            } else {
                Style::default().bg(Color::Rgb(30, 30, 40))
            };
            Row::new([
                Cell::from(format!("{:>4}", num)),
                Cell::from(e.timestamp.as_str()),
                Cell::from(e.src.as_str()),
                Cell::from(e.dst.as_str()),
                Cell::from(e.msg_type.as_str()),
                Cell::from(e.req_id.as_str()),
                Cell::from(e.payload.as_str()),
            ])
            .style(style)
        })
        .collect();

    let widths = [
        Constraint::Length(5),
        Constraint::Length(12),
        Constraint::Length(15),
        Constraint::Length(15),
        Constraint::Length(18),
        Constraint::Length(6),
        Constraint::Min(10),
    ];

    let table = Table::new(rows, widths)
        .header(header)
        .block(
            Block::default()
                .borders(Borders::ALL)
                .style(Style::default()),
        )
        .column_spacing(1);

    f.render_widget(table, chunks[1]);

    // Help bar
    let help = format!(
        " q:quit  ↑↓:scroll  PgUp/PgDn  Home/End  {} messages",
        app.message_count
    );
    let help_widget =
        Paragraph::new(help).style(Style::default().bg(Color::DarkGray).fg(Color::Gray));
    f.render_widget(help_widget, chunks[2]);
}

// ── TUI event loop ───────────────────────────────────────────────────────────

fn run_tui_loop(
    terminal: &mut Terminal<CrosstermBackend<io::Stdout>>,
    state: &Arc<Mutex<AppState>>,
    entry_rx: std::sync::mpsc::Receiver<LogEntry>,
) -> Result<()> {
    loop {
        // Process incoming messages (non-blocking)
        while let Ok(entry) = entry_rx.try_recv() {
            let mut app = state.lock().unwrap();
            app.entries.push(entry);
        }

        // Poll for input with timeout
        if event::poll(Duration::from_millis(50))? {
            if let Event::Key(key) = event::read()? {
                let mut app = state.lock().unwrap();
                let visible = terminal.size()?.height.saturating_sub(4) as usize;
                let total = app.entries.len().max(1);

                match key.code {
                    KeyCode::Char('q') | KeyCode::Esc => break,
                    KeyCode::Up | KeyCode::Char('k') => {
                        app.follow_bottom = false;
                        app.scroll = app.scroll.saturating_sub(1);
                    }
                    KeyCode::Down | KeyCode::Char('j') => {
                        if app.scroll + visible < total {
                            app.scroll += 1;
                        } else {
                            app.follow_bottom = true;
                        }
                    }
                    KeyCode::PageUp => {
                        app.follow_bottom = false;
                        app.scroll = app.scroll.saturating_sub(visible);
                    }
                    KeyCode::PageDown => {
                        app.scroll = (app.scroll + visible).min(total.saturating_sub(1));
                        if app.scroll + visible >= total {
                            app.follow_bottom = true;
                        }
                    }
                    KeyCode::Home => {
                        app.follow_bottom = false;
                        app.scroll = 0;
                    }
                    KeyCode::End => {
                        app.follow_bottom = true;
                    }
                    _ => {}
                }
            }
        }

        // Render
        terminal.draw(|f| {
            let app = state.lock().unwrap();
            render_ui(f, &app);
        })?;
    }

    Ok(())
}

// ── Main ─────────────────────────────────────────────────────────────────────

fn main() -> Result<()> {
    logger::init();
    let args = Args::parse();

    let port = args.port.unwrap_or(generated::MULTICAST_PORT as u16);
    let group: Ipv4Addr = args
        .group
        .parse()
        .with_context(|| format!("invalid multicast address: {}", args.group))?;
    let endpoint_name = args.endpoint.clone();
    let endpoint_id = fnv1a_32(&endpoint_name);
    let config_path = args.config.clone();

    // Build initial name map
    let name_map = build_name_map(&config_path);

    // Shared state
    let state = Arc::new(Mutex::new(AppState {
        name_map,
        endpoint_name: endpoint_name.clone(),
        endpoint_id,
        ..AppState::new()
    }));
    // Add our own name
    state.lock().unwrap().name_map.insert(endpoint_id, endpoint_name.clone());

    // Channel for network → TUI
    let (entry_tx, entry_rx) = std::sync::mpsc::channel::<LogEntry>();

    // Tokio runtime for network I/O
    let rt = tokio::runtime::Runtime::new()?;
    let net_state = state.clone();
    rt.spawn(async move {
        if let Err(e) = run_network(
            net_state,
            entry_tx,
            endpoint_name,
            group,
            port,
            config_path,
        )
        .await
        {
            log::error!("Network task failed: {}", e);
        }
    });

    // TUI setup
    enable_raw_mode()?;
    let mut stdout = io::stdout();
    execute!(stdout, EnterAlternateScreen, EnableMouseCapture)?;
    let backend = CrosstermBackend::new(stdout);
    let mut terminal = Terminal::new(backend)?;

    let result = run_tui_loop(&mut terminal, &state, entry_rx);

    // Cleanup
    disable_raw_mode()?;
    execute!(
        io::stdout(),
        LeaveAlternateScreen,
        DisableMouseCapture
    )?;

    result
}
