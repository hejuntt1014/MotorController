#include "Beeper.h"
#include "utils.h"

// 启动蜂鸣器响声
void Beeper::startBeep(int times, int duration, int interval) {
  beepTimes = times;
  beepDuration = duration;
  beepInterval = interval;
  currentBeep = 0;
  isBeeping = true;
  isOn = true;                // 开始时蜂鸣器为开启状态
  lastActionTime = millis();  // 记录启动时间
  speakerControl(true);       // 打开蜂鸣器
}

// 启动一次响声
void Beeper::startBeep(int duration) {
  startBeep(1, duration, 0);
}

// 在主循环中调用，用于更新蜂鸣器状态
void Beeper::update() {
  if (!isBeeping) return;  // 如果不在响声过程中，直接返回

  unsigned long currentTime = millis();

  if (isOn && (currentTime - lastActionTime >= beepDuration)) {
    // 响声时间结束，关闭蜂鸣器
    speakerControl(false);
    isOn = false;
    lastActionTime = currentTime;
    currentBeep++;

    // 判断是否完成所有响声
    if (currentBeep >= beepTimes) {
      isBeeping = false;  // 完成所有响声
    }
  } else if (!isOn && (currentTime - lastActionTime >= beepInterval)) {
    // 间隔时间结束，开始下一次响声
    if (currentBeep < beepTimes) {
      speakerControl(true);
      isOn = true;
      lastActionTime = currentTime;
    }
  }
}
