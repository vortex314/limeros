#ifndef _MAX31855_ACTOR_H_
#define _MAX31855_ACTOR_H_

#include <actor.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <msgs.h>

#ifndef MAX31855_SPI_HOST
#define MAX31855_SPI_HOST SPI2_HOST
#endif

#ifndef MAX31855_GPIO_SCK
#define MAX31855_GPIO_SCK GPIO_NUM_18
#endif

#ifndef MAX31855_GPIO_MISO
#define MAX31855_GPIO_MISO GPIO_NUM_19
#endif

#ifndef MAX31855_GPIO_CS
#define MAX31855_GPIO_CS GPIO_NUM_5
#endif

#ifndef MAX31855_SPI_CLOCK_HZ
#define MAX31855_SPI_CLOCK_HZ 250000
#endif

#ifndef MAX31855_SPI_MODE
#define MAX31855_SPI_MODE 0
#endif

#ifndef HEATER_SSR_GPIO
#define HEATER_SSR_GPIO GPIO_NUM_26
#endif

#ifndef HEATER_CONTROL_WINDOW_MS
#define HEATER_CONTROL_WINDOW_MS 2000
#endif

#ifndef HEATER_SETPOINT_MIN_C
#define HEATER_SETPOINT_MIN_C 20.0f
#endif

#ifndef HEATER_SETPOINT_MAX_C
#define HEATER_SETPOINT_MAX_C 320.0f
#endif

#ifndef HEATER_MAX_TEMP_C
#define HEATER_MAX_TEMP_C 350.0f
#endif

#ifndef HEATER_SENSOR_TIMEOUT_MS
#define HEATER_SENSOR_TIMEOUT_MS 3000
#endif

DEFINE_MSG(Max31855Read);


class Max31855Actor : public Actor
{
private:
  spi_device_handle_t _spi = nullptr;
  int _timer_poll = -1;
  int _timer_report = -1;
  uint32_t _poll_interval_ms = 200;
  uint32_t _report_interval_ms = 1000;
  gpio_num_t _pin_cs = MAX31855_GPIO_CS;
  gpio_num_t _pin_ssr = HEATER_SSR_GPIO;

  float _setpoint_c = 180.0f;
  float _kp = 3.0f;
  float _ki = 0.02f;
  float _kd = 0.0f;
  float _integral = 0.0f;
  float _prev_error = 0.0f;
  float _output_pct = 0.0f;
  float _last_temp_c = 0.0f;
  bool _enabled = false;
  bool _heater_on = false;
  bool _fault = false;
  bool _safety_trip = false;
  uint64_t _last_sensor_ok_ms = 0;
  uint64_t _last_pid_ms = 0;
  uint64_t _window_start_ms = 0;

public:
  Max31855Actor(const char *name, uint32_t poll_interval_ms = 500);
  ~Max31855Actor();
  void on_start() override;
  void on_message(const Envelope &env) override;

private:
  bool init_spi();
  bool read_sensor(float &thermocouple_c, float &internal_c, bool &fault, bool &open_circuit, bool &short_to_gnd, bool &short_to_vcc);
  void control();
  void publish_heating_event(uint64_t now_ms);
  void apply_pid(float current_temp_c, bool fault, uint64_t now_ms);
  void apply_ssr_output(uint64_t now_ms);
  void set_heater(bool on);
  bool is_setpoint_valid(float setpoint_c) const;
  static int32_t sign_extend(uint32_t value, uint8_t bit_count);
};

#endif
