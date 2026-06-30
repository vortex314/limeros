use crate::{
    my_window::WindowRegistration,
    window_gauge::WindowGauge,
    window_heater::WindowHeater,
    window_hoverboard::HoverboardWindow,
    window_max31855::WindowMax31855,
    window_plot::WindowPlot,
};

pub fn endpoint_window_registrations() -> Vec<WindowRegistration> {
    vec![
        WindowHeater::registration(),
        HoverboardWindow::registration(),
        WindowMax31855::registration(),
    ]
}

pub fn field_window_registrations() -> Vec<WindowRegistration> {
    vec![WindowPlot::registration(), WindowGauge::registration()]
}
