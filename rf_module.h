#ifndef RF_MODULE_H
#define RF_MODULE_H

#include <Arduino.h>
#include <RCSwitch.h>

// RF模块类
class RFModule {
public:
  void init();                  // 初始化RF模块
  void handleConfig();          // 处理RF配置
  void handleNormalMode();      // 处理RF正常模式
  bool isInConfigMode() const;  // 判断是否处于配置模式

private:
  RCSwitch rcSwitch;                     // RCSwitch对象
  bool inConfigMode = true;              // 是否处于配置模式
  unsigned long configStartTime = 0;     // 配置模式开始时间
  unsigned long storedRFCode = 0;        // 存储的RF代码
  unsigned long lastRFCodeTime = 0;      // 上次接收RF指令的时间
  unsigned long lastReceivedRFCode = 0;  // 上次接收到的RF代码
  unsigned long pressStartTime = 0;      // 按键按下开始时间
  bool isPressing = false;               // 是否正在按键

  void saveRFCode(unsigned long code);     // 保存RF代码
  void loadRFCode();                       // 加载RF代码
  bool isValidRFCode(unsigned long code);  // 判断RF代码是否有效
};

#endif  // RF_MODULE_H
