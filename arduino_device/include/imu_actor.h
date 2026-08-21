#ifndef IMU_ACTOR_H
#define IMU_ACTOR_H
#include <actor_framework.h>

#include "timer.h"
#include "messages.h"
#include "msgs.h"

// Clean up problematic macros defined by Energia/Arduino core
#undef min
#undef max
#undef abs
#undef round
#undef swap

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