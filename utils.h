#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "motor_state_machine.h"
#include <air001xx_hal_iwdg.h>

// GPIO 和电机控制相关的函数声明
void initGPIO();               // 初始化 GPIO
void speakerControl(bool on);  // 控制蜂鸣器状态
const char* getActionModeName(ActionMode mode);  // 获取动作模式名称

// 看门狗相关函数声明
void initWatchdog();     // 初始化看门狗
void refreshWatchdog();  // 看门狗喂狗

// 判断 unsigned long 类型的值是否有效
bool isValidUnsignedLong(unsigned long value);

#endif  // UTILS_H
