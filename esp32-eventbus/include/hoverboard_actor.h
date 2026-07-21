#ifndef _HOVERBOARD_ACTOR_H_
#define _HOVERBOARD_ACTOR_H_
#include <actor.h>
#include <functional>
#include <vector>
#include <option.h>
#include <msgs.h>
#include <driver/uart.h>
#include <mc_actor.h>

class UartRxd : public Msg
{
public:
    static const uint32_t MSG_ID = FNV("UartRxd");
    static constexpr const char *MSG_NAME = "UartRxd";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
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
    HoverboardActor(const char *name);
    ~HoverboardActor();
    void on_message(const ActorMessage &msg);
    void on_timer(int timer_id);
    void on_start();
    Result<bool> init_uart();
    void write_uart(const Buffer &);
    void handle_uart_bytes(const Buffer &);
    static Result<Buffer> cobs_decode(const Buffer &input);
    static Result<Buffer> check_crc(const Buffer &input);
    static Result<HoverboardEvent *> parse_info_msg(const Buffer &input);
};

#endif
