
#include <motor_actor.h>

extern "C" bool capture_callback(
    mcpwm_cap_channel_t *cap_channel,
    const mcpwm_capture_event_data_t *capture_event,
    void *arg);

MotorActor::MotorActor(): Actor()
{
    _timer_publish = timer_repetitive(300);
    _timer_pid = timer_repetitive(100);
    _Kd = 0.0f;
    _Kp = 0.05f;
    _Ki = 0.003f;
    _rpm_target = 2000.0f;
    _rpm_measured = 0.0f;
    _pwm_value = 350;
    _pwm_percent = 20.0f;
    _last_rpm_measured = 0;
    _rpm_integral = 0.0f;
}

float MotorActor::pid_update(float delta_t, float error)
{
    if (delta_t <= 0.0f)
    {
        return _p + _i;
    }

    _p = _Kp * error;
    _rpm_integral += error * delta_t;
    if (_Ki != 0.0f)
    {
        _rpm_integral = clip(_rpm_integral, -50.0f / _Ki, 50.0f / _Ki);
    }
    _i = _Ki * _rpm_integral;
    _d = _Kd * (error - _previous_error) / delta_t;

    DEBUG("error:%f p:%f i:%f d:%f", error, _p, _i, _d);
    _previous_error = error;
    return _p + _i + _d;
}


void MotorActor::setup(void)
{
    INFO("MotorActor setup");
    Res init_res = pwm_init()
        .and_then([&](auto r)
                  { return capture_init(); });
    init_res.just([&](auto v)
                  { INFO(" Motor Actor setup complete."); });
    init_res.just_err([&](auto err)
                      { ERROR("Motor Actor setup failed: %d %s", err.rc, err.msg); });
}

void MotorActor::receive(const ActorMessage &msg)
{
    msg.handle<TimerMsg>([&](const TimerMsg &timer_msg)
                         { on_timer(timer_msg.timer_id); });
    msg.handle<MotorRequest>([&](const auto &cmd)
                             {
        INFO("Motor actor cmd ");
        cmd.rpm_target.handle_if([&](auto v) { _rpm_target = v; });
        publish_props(); });
}

void MotorActor::publish_props()
{
    INFO("Motor capture pulses=%u rpm=%f target=%f pwm=%f",
         _last_capture_count, _rpm_measured, _rpm_target, _pwm_percent);
    MotorEvent *evt = new MotorEvent();
    evt->rpm_measured = _rpm_measured;
    evt->rpm_target = _rpm_target;
    evt->pwm = _pwm_percent;
    evt->Kp = _Kp;
    evt->Ki = _Ki;
    evt->Kd = _Kd;

    emit(evt);
}

void MotorActor::on_timer(int id)
{
    if (id == _timer_publish)
    {
        publish_props();
    }
    else if (id == _timer_pid)
    {
        uint32_t capture_sum;
        uint32_t capture_count;
        portENTER_CRITICAL(&_capture_mux);
        capture_sum = _capture_sum;
        capture_count = _capture_count;
        _capture_sum = 0;
        _capture_count = 0;
        portEXIT_CRITICAL(&_capture_mux);
        _last_capture_count = capture_count;

        float delta_t = static_cast<float>(esp_timer_get_time() - _last_rpm_measured) / 1'000'000.0f; // Convert microseconds to seconds
        if (capture_count > 0 && capture_sum > 0)
        {
            const float average_ticks = static_cast<float>(capture_sum) / capture_count;
            const float hz = 80'000'000.0f / average_ticks;
            _rpm_measured = (hz * 60.0f) / 4.0f;
            delta_t = static_cast<float>(capture_sum) / 80'000'000.0f;
            _last_rpm_measured = esp_timer_get_time();
        }
        else if (delta_t > 0.2f)
        {
            DEBUG("No recent rpm measurements, delta_t:%f sec", delta_t);
            _rpm_measured = 0.0f;
        }

        if (delta_t <= 0.0f)
        {
            return;
        }

        float pid = pid_update(delta_t, _rpm_target - _rpm_measured);
        _pwm_percent = MotorActor::clip(pid, 0, 100.0f);
        _pwm_value = MotorActor::pwm_percent_to_ticks(_pwm_percent);

        if (_pwm_value > TICKS_PER_PERIOD)
        {
            _pwm_value = TICKS_PER_PERIOD;
        }

        DEBUG("pid:%f pwm:%f pwm_value:%d", pid, _pwm_percent, _pwm_value);

        esp_err_t err = mcpwm_comparator_set_compare_value(_cmpr, _pwm_value);
        if (err != ESP_OK)
        {
            ERROR("Failed to set compare value: %s", esp_err_to_name(err));
        }
    }
}



MotorActor::~MotorActor()
{
    INFO("Stopping Motor actor");
}

/**
 * @brief Initializes the PWM (Pulse Width Modulation) for the MotorActor.
 *
 * This function configures and initializes the MCPWM (Motor Control Pulse Width Modulation) timer, operator, comparator,
 * and generator to control the motor's PWM signals. It sets up the necessary configurations for the timer, operator,
 * comparator, and generator, and defines the actions for timer and compare events.
 *
 * @return ResOk on successful initialization, or an appropriate error code if initialization fails.
 *
 * Configuration Details:
 * - Timer:
 *   - Group ID: 0
 *   - Clock Source: Default (160 MHz)
 *   - Resolution: 10 MHz (1 tick = 0.1 µs)
 *   - Count Mode: Up
 *   - Period: 50 µs (20 kHz)
 *   - Interrupt Priority: 1
 *   - Flags:
 *     - Update period on empty and sync events.
 *     - Power-down mode disabled.
 *
 * - Operator:
 *   - Group ID: 0
 *   - Interrupt Priority: 1
 *   - Flags:
 *     - Update generator actions and dead time on timer zero, peak, and sync events.
 *
 * - Comparator:
 *   - Interrupt Priority: 1
 *   - Flags:
 *     - Update compare value on timer zero, peak, and sync events.
 *
 * - Generator:
 *   - GPIO Pin: Configured for PWM output.
 *   - Flags:
 *     - Pull-up enabled.
 *     - Pull-down and open-drain mode disabled.
 *     - PWM signal inversion disabled.
 *
 * Actions:
 * - Sets the GPIO high when the timer event is empty.
 * - Sets the GPIO low when the compare threshold is reached.
 *
 * Additional Notes:
 * - The GPIO pin for PWM is configured using `config_gpio_to_value`.
 * - The timer is started without stopping after initialization.
 */
Res MotorActor::pwm_init()
{
    mcpwm_timer_config_t timer_config = mcpwm_timer_config_t{
        group_id : 0,
        clk_src : MCPWM_TIMER_CLK_SRC_DEFAULT,          // 160MHz PLL
        resolution_hz : BLDC_MCPWM_TIMER_RESOLUTION_HZ, // 10MHz, 1 tick = 0.1us
        count_mode : MCPWM_TIMER_COUNT_MODE_UP,
        period_ticks : TICKS_PER_PERIOD, // 50us, 20KHz, 500 ticks
        intr_priority : 1,
        flags : {
            update_period_on_empty : 1, // change
            update_period_on_sync : 1,
        //    allow_pd : 0, // set to allow power down. When this flag set, the driver will backup/restore the MCPWM registers before/after entering/exist sleep mode.
        },                // when to change the value
    };

    CHECK_ESP(mcpwm_new_timer(&timer_config, &_timer));
    CHECK_ESP(mcpwm_timer_enable(_timer));

    // define operator

    mcpwm_operator_config_t operator_config = mcpwm_operator_config_t{
        group_id : 0,
        intr_priority : 1,
        flags : {
            update_gen_action_on_tez : 0,  // update generator action when timer counts to zero
            update_gen_action_on_tep : 1,  // update generator action when timer counts to peak
            update_gen_action_on_sync : 0, // update generator action on sync event

            update_dead_time_on_tez : 0,  // update dead time when timer counts to zero
            update_dead_time_on_tep : 0,  // update dead time when timer counts to peak
            update_dead_time_on_sync : 0, // update dead time on sync event
        },                                // when to change the value
    };
    CHECK_ESP(mcpwm_new_operator(&operator_config, &_oper));
    CHECK_ESP(mcpwm_operator_connect_timer(_oper, _timer));

    mcpwm_comparator_config_t comparator_config = mcpwm_comparator_config_t{
        intr_priority : 1,
        flags : {
            update_cmp_on_tez : 0,  // update compare value on timer event zero
            update_cmp_on_tep : 1,  // update compare value on timer event peak
            update_cmp_on_sync : 0, // update compare value on sync event

        },
    };
    CHECK_ESP(mcpwm_new_comparator(_oper, &comparator_config, &_cmpr));

    TR(config_gpio_to_value(GPIO_PWM_2, 1)); // value 0 leads to high spike voltage , as the induction current has no way to go

    mcpwm_generator_config_t generator_config = mcpwm_generator_config_t{
        gen_gpio_num : GPIO_PWM_1,
        flags : {
            invert_pwm : 0,   // invert the PWM signal (done by GPIO matrix)
            io_loop_back : 0, // For debug/test, the signal output from the GPIO will be fed to the input path as well
            io_od_mode : 0,   // Configure the GPIO as open-drain mode
            pull_up : 1,      // pull up the GPIO
            pull_down : 0,    // pull down the GPIO
        },
    };
    CHECK_ESP(mcpwm_new_generator(_oper, &generator_config, &_gen));

    CHECK_ESP(mcpwm_comparator_set_compare_value(_cmpr, _pwm_value));
    // at start set GPIO high
    CHECK_ESP(mcpwm_generator_set_action_on_timer_event(
        _gen,
        mcpwm_gen_timer_event_action_t{
            direction : MCPWM_TIMER_DIRECTION_UP,
            event : MCPWM_TIMER_EVENT_EMPTY,
            action : MCPWM_GEN_ACTION_HIGH
        }));
    // go low on GPIO on compare threshold
    CHECK_ESP(mcpwm_generator_set_action_on_compare_event(
        _gen,
        mcpwm_gen_compare_event_action_t{
            direction : MCPWM_TIMER_DIRECTION_UP,
            comparator : _cmpr,
            action : MCPWM_GEN_ACTION_LOW
        }));

    //  CHECK_ESP(mcpwm_timer_enable(self.timer_handle));
    CHECK_ESP(mcpwm_timer_start_stop(_timer, MCPWM_TIMER_START_NO_STOP));

    return Res::Ok(true);
}

/**
 * @brief Initializes the motor capture functionality.
 *
 * This method configures and initializes the MCPWM capture timer and channel
 * for capturing PWM signals. It sets up the timer, configures the capture
 * channel with specific GPIO and edge detection settings, and registers the
 * callback for handling capture events. Finally, it enables and starts the
 * capture timer and channel.
 *
 * @return ResOk if the initialization is successful, otherwise an error code.
 */
Res MotorActor::capture_init()
{
    mcpwm_capture_timer_config_t cap_timer_config;
    bzero(&cap_timer_config, sizeof(cap_timer_config));
    cap_timer_config.group_id = 0;
    cap_timer_config.clk_src = MCPWM_CAPTURE_CLK_SRC_APB;
    cap_timer_config.resolution_hz = 80'000'000; // 80 MHz

    CHECK_ESP(mcpwm_new_capture_timer(&cap_timer_config, &_cap_timer));

    mcpwm_capture_channel_config_t cap_channel_config = mcpwm_capture_channel_config_t{
        gpio_num : GPIO_CAPTURE,
        intr_priority : 1,
        prescale : 1,
        flags : {
            pos_edge : 0, // capture on positive edge
            neg_edge : 1, // capture on negative edge
            pull_up : 1,
            pull_down : 0,            // pull up the GPIO
            invert_cap_signal : 0,    // Invert the input capture signal
            io_loop_back : 0,         // For debug/test, the signal output from the GPIO will be fed to the input path as well
            keep_io_conf_at_exit : 0, // Deprecated. Driver won't change the GPIO configuration in deinilization.
        },
    };

    CHECK_ESP(mcpwm_new_capture_channel(_cap_timer, &cap_channel_config, &_cap_channel));

    // TaskHandle_t task_to_notify = xTaskGetCurrentTaskHandle();
    mcpwm_capture_event_callbacks_t cbs = mcpwm_capture_event_callbacks_t{
        on_cap : capture_callback
    };

    CHECK_ESP(mcpwm_capture_channel_register_event_callbacks(
        _cap_channel,
        &cbs,
        this));
    // activate capture channel
    CHECK_ESP(mcpwm_capture_channel_enable(_cap_channel));
    CHECK_ESP(mcpwm_capture_timer_enable(_cap_timer));
    CHECK_ESP(mcpwm_capture_timer_start(_cap_timer));

    return Res::Ok(true);
}

Res config_gpio_to_value(gpio_num_t gpio_num, uint8_t value)
{
    gpio_config_t io_conf = gpio_config_t{
        pin_bit_mask : (uint64_t)1 << gpio_num,
        mode : GPIO_MODE_OUTPUT,
        pull_up_en : GPIO_PULLUP_DISABLE,
        pull_down_en : GPIO_PULLDOWN_DISABLE,
        intr_type : GPIO_INTR_DISABLE,
    };
    CHECK_ESP(gpio_config(&io_conf));
    CHECK_ESP(gpio_set_level(gpio_num, value));
    return Res::Ok(true);
}

/**
 * @brief Callback function for handling capture events from an MCPWM capture channel.
 *
 * This function is invoked when a capture event occurs on the specified MCPWM capture channel.
 * It calculates the time period between consecutive capture events, accumulates the total
 * time period, and counts the number of events. When the accumulated time exceeds a threshold,
 * it sends a message to the associated MotorActor instance with the captured data.
 *
 * @param cap_channel Pointer to the MCPWM capture channel that triggered the event.
 * @param capture_event Pointer to the data structure containing details about the capture event,
 *                      including the captured value and edge type.
 * @param arg Pointer to user-defined data passed to the callback. In this case, it is expected
 *            to be a pointer to a MotorActor instance.
 *
 * @return Always returns true to indicate successful handling of the capture event.
 */

extern "C" bool capture_callback(
    mcpwm_cap_channel_t *cap_channel,
    const mcpwm_capture_event_data_t *capture_event,
    void *arg)
{
    static_cast<MotorActor *>(arg)->record_capture_from_isr(capture_event->cap_value);
    return true;
}

void MotorActor::record_capture_from_isr(uint32_t capture_value)
{
    portENTER_CRITICAL_ISR(&_capture_mux);
    const uint32_t ticks_per_period = capture_value - _last_capture;
    _last_capture = capture_value;
    if (ticks_per_period != 0)
    {
        _capture_sum += ticks_per_period;
        ++_capture_count;
    }
    portEXIT_CRITICAL_ISR(&_capture_mux);
}

uint32_t MotorActor::pwm_percent_to_ticks(float percent)
{
    float ticks = 500.0f - ((percent * static_cast<float>(TICKS_PER_PERIOD)) / 100.0f);
    return static_cast<uint32_t>(ticks);
}

float MotorActor::clip(float value, float min, float max)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    else
    {
        return value;
    }
}

Res MotorActor::pwm_stop()
{
    _rpm_target = 0.0f;
    _pwm_percent = 0.0f;
    _pwm_value = pwm_percent_to_ticks(_pwm_percent);
    if (_cmpr != nullptr)
    {
        CHECK_ESP(mcpwm_comparator_set_compare_value(_cmpr, _pwm_value));
    }
    return Res::Ok(true);
}

