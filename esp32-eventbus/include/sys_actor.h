#ifndef _SYS_ACTOR_H_
#define _SYS_ACTOR_H_
#include <actor.h>
#include <functional>
#include <vector>
#include <option.h>
#include <msgs.h>


class SysActor : public Actor
{
private:
  int _timer_publish = -1;
  int _prop_counter = 0;
  std::shared_ptr<SysEvent> _sys_event = std::make_shared<SysEvent>();

public:
  SysActor();
  ~SysActor();
  void receive(const ActorMessage &msg);
  void on_timer(int timer_id);
  void setup();
  void set_utc(int64_t utc);
  void publish_info();
  static void reboot(bool b);
};

#endif
