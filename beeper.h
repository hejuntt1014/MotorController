#ifndef BEEPER_H
#define BEEPER_H

#include <Arduino.h>

// 管理蜂鸣器响声的类
class Beeper {
public:
  void startBeep(int times, int duration, int interval);  // 启动响声
  void startBeep(int duration);                           // 启动一次响声
  void update();                                          // 在主循环中调用，用于更新响声状态

private:
  int beepTimes = 0;                 // 总响声次数
  int currentBeep = 0;               // 当前响声计数
  int beepDuration = 0;              // 每次响声持续时间
  int beepInterval = 0;              // 每次响声之间的间隔
  bool isBeeping = false;            // 是否在响声过程中
  bool isOn = false;                 // 追踪蜂鸣器当前是否开启
  unsigned long lastActionTime = 0;  // 上次动作的时间
};

#endif  // BEEPER_H
