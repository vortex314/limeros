
#include "imu_actor.h"
#include <msgs.h>

ImuActor::ImuActor(ActorId gateway) : _timer(1), _imu_msg(nullptr),
                                      _imu_event(nullptr), _from_device_msg(nullptr), _gateway_actor_id(gateway) {}

void ImuActor::setup()
{
    _timer.setOwner(id);
    _timer.start(500, true); // Start the timer with a 100 msec
    _imu_event = new ImuEvent();
    _from_device_msg = new FromDevice(_imu_event);
    _imu_msg = std::make_shared<ActorMessage>();
    _imu_msg->src = id;
    _imu_msg->dst = _gateway_actor_id; // Assuming 0 is the destination for IMU events, you may want to change this to the appropriate actor ID.
    _imu_msg->set_payload(_from_device_msg);
}

void ImuActor::receive(std::shared_ptr<ActorMessage> msg)
{

    msg->handle<TimerMsg>([this](const TimerMsg &m)
                          {
                            FromDevice from_device_msg(_imu_event);
                             if (m.timer_id == _timer.id())
                             {
                                INFO("IMU Timer expired, sending IMU event");
                                _imu_event->gyro_x = 1.0f;
                                _imu_event->gyro_y = 2.0f;
                                _imu_event->gyro_z = 3.0f;
                                actor_system.send(_imu_msg);

                             } });
}
void ImuActor::update()
{
    _timer.update();
}
