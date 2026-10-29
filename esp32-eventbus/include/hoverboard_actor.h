#ifndef _HOVERBOARD_ACTOR_H_
#define _HOVERBOARD_ACTOR_H_
#include <actor.h>
#include <functional>
#include <vector>
#include <option.h>
#include <msgs.h>
#include <driver/uart.h>
#include <serial_actor.h>

class UartRxd : public ActorPayload
{
TYPED_MESSAGE(UartRxd)
    Bytes payload;
    UartRxd(const Bytes &payload) : payload(payload) {};
};


class HoverboardActor : public Actor
{
private:
    int _timer_publish = -1;
    int _timer_hb_alive = -1;
    int _prop_counter = 0;
    uint16_t _speed = 0;
    uint16_t _steer = 0;

public:
    QueueHandle_t uart_queue = NULL;
    TaskHandle_t uart_task_handle = NULL;
    static const int UART_PORT = UART_NUM_1;
    static const int UART_BUF_SIZE = 1024;
    Buffer uart_read_buffer;

public:
    HoverboardActor();
    ~HoverboardActor();
    void receive(const ActorMessage &msg);
    void on_timer(int timer_id);
    void setup();
    Result<bool> init_uart();
    void write_uart(const Buffer &);
    void handle_uart_bytes(const Buffer &);
    static Result<Buffer> cobs_decode(const Buffer &input);
    static Result<Buffer> check_crc(const Buffer &input);
    static Result<HoverboardEvent *> parse_info_msg(Buffer &input);
};

#endif
