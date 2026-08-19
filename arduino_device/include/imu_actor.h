#ifndef IMU_ACTOR_H
#define IMU_ACTOR_H
#include <actor_framework.h>
#include <Arduino.h>
#include "timer.h"
#include "messages.h"
#include "msgs.h"

class ImuActor : public Actor
{
private:
    Timer _timer;
    std::shared_ptr<ActorMessage> _imu_msg;
    ImuEvent* _imu_event;
    FromDevice* _from_device_msg;
    ActorId _gateway_actor_id;


public:
    explicit ImuActor(ActorId gateway);
    void setup() override;
    void update() override;
    void receive(std::shared_ptr<ActorMessage> msg) override;
};

#endif // IMU_ACTOR_H