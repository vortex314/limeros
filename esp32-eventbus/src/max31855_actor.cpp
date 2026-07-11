#include <max31855_actor.h>
#include <esp_err.h>
#include <cmath>
#include <esp_rom_sys.h>

static float clampf(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

Max31855Actor::Max31855Actor(const char *name, uint32_t poll_interval_ms)
    : Actor(name), _poll_interval_ms(poll_interval_ms)
{
    _setpoint_c = clampf(_setpoint_c, HEATER_SETPOINT_MIN_C, HEATER_SETPOINT_MAX_C);
    _timer_poll = timer_repetitive(_poll_interval_ms);
    _timer_report = timer_repetitive(_report_interval_ms);
}

Max31855Actor::~Max31855Actor()
{
    set_heater(false);

    if (_spi != nullptr)
    {
        esp_err_t rc = spi_bus_remove_device(_spi);
        if (rc != ESP_OK)
        {
            WARN("%s: failed removing SPI device: %s", name(), esp_err_to_name(rc));
        }
        _spi = nullptr;
    }

    esp_err_t bus_rc = spi_bus_free(MAX31855_SPI_HOST);
    if (bus_rc != ESP_OK && bus_rc != ESP_ERR_INVALID_STATE)
    {
        WARN("%s: failed freeing SPI bus: %s", name(), esp_err_to_name(bus_rc));
    }
}

void Max31855Actor::on_start()
{
    if (!init_spi())
    {
        ERROR("%s: MAX31855 SPI init failed", name());
        timer_stop(_timer_poll);
        return;
    }
    DeviceAliveEvent *alive_event = new DeviceAliveEvent();
    alive_event->publishes.push_back(Max31855Event::name_value);
    alive_event->publishes.push_back(HeatingEvent::name_value);
    alive_event->subscribes.push_back(Max31855Read::name_value);
    alive_event->subscribes.push_back(HeatingRequest::name_value);
    emit(alive_event);

    gpio_set_direction(_pin_ssr, GPIO_MODE_OUTPUT);
    set_heater(false);
    _last_pid_ms = esp_timer_get_time() / 1000;
    _window_start_ms = _last_pid_ms;
    _last_sensor_ok_ms = _last_pid_ms;

    INFO("Heating actor started: poll=%u ms, SCK=%d MISO=%d CS=%d SSR=%d clk=%d mode=%d",
         _poll_interval_ms,
         MAX31855_GPIO_SCK,
         MAX31855_GPIO_MISO,
         _pin_cs,
         _pin_ssr,
         MAX31855_SPI_CLOCK_HZ,
         MAX31855_SPI_MODE);
}

void Max31855Actor::on_message(const Envelope &env)
{
    const Msg &msg = *env.msg;

    msg.handle<TimerMsg>([&](const TimerMsg &timer_msg)
                         {
    if (timer_msg.timer_id == _timer_poll)
    {
            control();
    } 

    else if (timer_msg.timer_id == _timer_report)
    {
            publish_heating_event(esp_timer_get_time() / 1000);
    } });

    msg.handle<Max31855Read>([&](auto _)
                                                         { control(); });

        msg.handle<HeatingRequest>([&](const HeatingRequest &req)
                                                             {
            if (req.setpoint_c)
            {
                const float clamped = clampf(*req.setpoint_c, HEATER_SETPOINT_MIN_C, HEATER_SETPOINT_MAX_C);
                if (clamped != *req.setpoint_c)
                {
                    WARN("Setpoint %.2fC out of range, clamped to %.2fC", *req.setpoint_c, clamped);
                }
                _setpoint_c = clamped;
            }
                if (req.enabled) _enabled = *req.enabled;
                if (req.kp) _kp = *req.kp;
                if (req.ki) _ki = *req.ki;
                if (req.kd) _kd = *req.kd;
                if (req.reset_integral && *req.reset_integral)
                {
                        _integral = 0.0f;
                        _prev_error = 0.0f;
                }
                if (!_enabled)
                {
                        _output_pct = 0.0f;
                        set_heater(false);
                }
                if (req.enabled && *req.enabled)
                {
                    _safety_trip = false;
                    _fault = false;
                    _last_sensor_ok_ms = esp_timer_get_time() / 1000;
                }
                INFO("HeatingRequest: setpoint=%.2f enabled=%d kp=%.3f ki=%.3f kd=%.3f",
                         _setpoint_c,
                         _enabled,
                         _kp,
                         _ki,
                         _kd);
        });
}

bool Max31855Actor::init_spi()
{
    spi_bus_config_t bus_cfg = {};
    bus_cfg.sclk_io_num = MAX31855_GPIO_SCK;
    bus_cfg.mosi_io_num = -1;
    bus_cfg.miso_io_num = MAX31855_GPIO_MISO;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 4;

    esp_err_t rc = spi_bus_initialize(MAX31855_SPI_HOST, &bus_cfg, SPI_DMA_DISABLED);
    if (rc != ESP_OK && rc != ESP_ERR_INVALID_STATE)
    {
        ERROR("%s: spi_bus_initialize failed: %s", name(), esp_err_to_name(rc));
        return false;
    }

    gpio_config_t cs_cfg = {};
    cs_cfg.pin_bit_mask = 1ULL << static_cast<uint64_t>(_pin_cs);
    cs_cfg.mode = GPIO_MODE_OUTPUT;
    cs_cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cs_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cs_cfg.intr_type = GPIO_INTR_DISABLE;
    rc = gpio_config(&cs_cfg);
    if (rc != ESP_OK)
    {
        ERROR("%s: gpio_config(CS) failed: %s", name(), esp_err_to_name(rc));
        return false;
    }
    gpio_set_level(_pin_cs, 1);

    spi_device_interface_config_t dev_cfg = {};
    dev_cfg.clock_speed_hz = MAX31855_SPI_CLOCK_HZ;
    dev_cfg.mode = MAX31855_SPI_MODE;
    dev_cfg.spics_io_num = -1;
    dev_cfg.queue_size = 1;
    dev_cfg.flags = SPI_DEVICE_HALFDUPLEX;

    rc = spi_bus_add_device(MAX31855_SPI_HOST, &dev_cfg, &_spi);
    if (rc != ESP_OK)
    {
        ERROR("%s: spi_bus_add_device failed: %s", name(), esp_err_to_name(rc));
        return false;
    }
    INFO("%s: MAX31855 SPI initialized successfully", name());
    return true;
}

bool Max31855Actor::read_sensor(float &thermocouple_c, float &internal_c, bool &fault, bool &open_circuit, bool &short_to_gnd, bool &short_to_vcc)
{
    if (_spi == nullptr)
    {
        return false;
    }

    uint8_t rx_data[4] = {0, 0, 0, 0};
    spi_transaction_t trans = {};
    trans.length = 32;
    trans.rxlength = 32;
    trans.rx_buffer = rx_data;

    gpio_set_level(_pin_cs, 0);
    esp_rom_delay_us(2);
    esp_err_t rc = spi_device_transmit(_spi, &trans);
    esp_rom_delay_us(2);
    gpio_set_level(_pin_cs, 1);
    if (rc != ESP_OK)
    {
        ERROR("%s: spi_device_transmit failed: %s", name(), esp_err_to_name(rc));
        return false;
    }

    const uint32_t raw = (static_cast<uint32_t>(rx_data[0]) << 24) |
                         (static_cast<uint32_t>(rx_data[1]) << 16) |
                         (static_cast<uint32_t>(rx_data[2]) << 8) |
                         (static_cast<uint32_t>(rx_data[3]));

    if (raw == 0x00000000UL || raw == 0xFFFFFFFFUL)
    {
        WARN("%s: suspicious MAX31855 raw frame 0x%08lX (check wiring/pins/ground)", name(), static_cast<unsigned long>(raw));
    }

    fault = (raw & (1UL << 16)) != 0;
    open_circuit = (raw & (1UL << 0)) != 0;
    short_to_gnd = (raw & (1UL << 1)) != 0;
    short_to_vcc = (raw & (1UL << 2)) != 0;

    const int32_t tc_raw = sign_extend((raw >> 18) & 0x3FFF, 14);
    const int32_t cj_raw = sign_extend((raw >> 4) & 0x0FFF, 12);

    thermocouple_c = static_cast<float>(tc_raw) * 0.25f;
    internal_c = static_cast<float>(cj_raw) * 0.0625f;

    DEBUG("%s: raw=0x%08lX tc=%.2fC cj=%.2fC fault=%d oc=%d scg=%d scv=%d",
          name(),
          static_cast<unsigned long>(raw),
          thermocouple_c,
          internal_c,
          fault,
          open_circuit,
          short_to_gnd,
          short_to_vcc);

    return true;
}

void Max31855Actor::control()
{
    const uint64_t now_ms = esp_timer_get_time() / 1000;
    float thermocouple_c = 0.0f;
    float internal_c = 0.0f;
    bool fault = false;
    bool open_circuit = false;
    bool short_to_gnd = false;
    bool short_to_vcc = false;

    if (!read_sensor(thermocouple_c, internal_c, fault, open_circuit, short_to_gnd, short_to_vcc))
    {
        WARN("%s: failed to read MAX31855", name());
        if (now_ms - _last_sensor_ok_ms > HEATER_SENSOR_TIMEOUT_MS)
        {
            if (!_safety_trip)
            {
                ERROR("%s: safety trip - sensor timeout (%llu ms)", name(), now_ms - _last_sensor_ok_ms);
            }
            _safety_trip = true;
            _enabled = false;
        }
        _fault = _safety_trip;
        _output_pct = 0.0f;
        set_heater(false);
        publish_heating_event(now_ms);
        return;
    }

    // ArduinoJson serializes NaN as null, so sanitize before emitting.
    if (!std::isfinite(thermocouple_c))
    {
        WARN("%s: thermocouple value is NaN/Inf, forcing 0.0", name());
        thermocouple_c = 0.0f;
    }
    if (!std::isfinite(internal_c))
    {
        WARN("%s: internal value is NaN/Inf, forcing 0.0", name());
        internal_c = 0.0f;
    }

    auto event = new Max31855Event();

    event->thermocouple_temp = thermocouple_c;
    event->internal_temp     = internal_c;
    event->fault = fault;
    event->fault_open_tc = open_circuit;
    event->fault_short_gnd = short_to_gnd;
    event->fault_short_vcc = short_to_vcc;
 //   event->timestamp_ms = esp_timer_get_time() / 1000;

    DEBUG("MAX31855 tc=%.2fC cj=%.2fC fault=%d oc=%d scg=%d scv=%d",
         thermocouple_c,
         internal_c,
         fault,
         open_circuit,
         short_to_gnd,
         short_to_vcc);

    emit(event);

    const bool sensor_fault = fault || open_circuit || short_to_gnd || short_to_vcc;
    if (!sensor_fault)
    {
        _last_sensor_ok_ms = now_ms;
    }

    _last_temp_c = thermocouple_c;
    if (_last_temp_c >= HEATER_MAX_TEMP_C)
    {
        if (!_safety_trip)
        {
            ERROR("%s: safety trip - overtemperature %.2fC >= %.2fC", name(), _last_temp_c, HEATER_MAX_TEMP_C);
        }
        _safety_trip = true;
        _enabled = false;
    }

    _fault = sensor_fault || _safety_trip;
    apply_pid(_last_temp_c, _fault, now_ms);
    apply_ssr_output(now_ms);
}

void Max31855Actor::apply_pid(float current_temp_c, bool fault, uint64_t now_ms)
{
    if (!_enabled || fault)
    {
        _output_pct = 0.0f;
        _integral = 0.0f;
        _prev_error = 0.0f;
        _last_pid_ms = now_ms;
        return;
    }

    float dt_sec = static_cast<float>(now_ms - _last_pid_ms) / 1000.0f;
    if (dt_sec <= 0.0f)
    {
        dt_sec = static_cast<float>(_poll_interval_ms) / 1000.0f;
    }
    _last_pid_ms = now_ms;

    const float error = _setpoint_c - current_temp_c;
    _integral += error * dt_sec;
    _integral = clampf(_integral, -1000.0f, 1000.0f);
    const float derivative = (error - _prev_error) / dt_sec;
    _prev_error = error;

    const float control = (_kp * error) + (_ki * _integral) + (_kd * derivative);
    _output_pct = clampf(control, 0.0f, 100.0f);
}

void Max31855Actor::apply_ssr_output(uint64_t now_ms)
{
    if (!_enabled || _fault)
    {
        set_heater(false);
        return;
    }

    const uint32_t window_ms = HEATER_CONTROL_WINDOW_MS;
    if (now_ms - _window_start_ms >= window_ms)
    {
        _window_start_ms = now_ms;
    }

    const uint64_t elapsed = now_ms - _window_start_ms;
    const uint64_t on_time = static_cast<uint64_t>((_output_pct / 100.0f) * window_ms);
    set_heater(elapsed < on_time);
}

void Max31855Actor::publish_heating_event(uint64_t now_ms)
{
    auto event = new HeatingEvent();
    event->temperature_c = _last_temp_c;
    event->setpoint_c = _setpoint_c;
    event->output_pct = _output_pct;
    event->enabled = _enabled;
    event->heater_on = _heater_on;
    event->fault = _fault;
    event->timestamp_ms = now_ms;
    emit(event);

    INFO("HEATING temp=%.2fC setpoint=%.2fC out=%.1f%% heater=%d enabled=%d fault=%d",
         _last_temp_c,
         _setpoint_c,
         _output_pct,
         _heater_on,
         _enabled,
         _fault);
}

void Max31855Actor::set_heater(bool on)
{
    _heater_on = on;
    gpio_set_level(_pin_ssr, _heater_on ? 1 : 0); // SSR driven via 
}

bool Max31855Actor::is_setpoint_valid(float setpoint_c) const
{
    return setpoint_c >= HEATER_SETPOINT_MIN_C && setpoint_c <= HEATER_SETPOINT_MAX_C;
}

int32_t Max31855Actor::sign_extend(uint32_t value, uint8_t bit_count)
{
    const uint32_t sign_bit = 1UL << (bit_count - 1);
    if (value & sign_bit)
    {
        const uint32_t mask = ~((1UL << bit_count) - 1);
        return static_cast<int32_t>(value | mask);
    }
    return static_cast<int32_t>(value);
}
