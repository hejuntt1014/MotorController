// button_handling.h
#ifndef BUTTON_HANDLING_H
#define BUTTON_HANDLING_H

#include <Arduino.h>

// 按钮状态结构体定义
struct ButtonState {
  bool btn1Pressed = false;
  bool btn2Pressed = false;
  bool pressureSensorTriggered = false;
  bool antiPinchTriggered = false;
};

// 外部声明按钮状态
extern ButtonState buttonState;


void onBtn1Press();              // 按钮1按下事件处理函数
void onBtn1Release();            // 按钮1释放事件处理函数
void onBtn2Press();              // 按钮2按下事件处理函数
void onBtn2Release();            // 按钮2释放事件处理函数
void onPressureSensorPress();    // 压力传感器按下事件处理函数
void onPressureSensorRelease();  // 压力传感器释放事件处理函数
void onAntiPinchPress();         // 防夹传感器按下事件处理函数
void onAntiPinchRelease();       // 防夹传感器释放事件处理函数


// 初始化按钮设置
void buttonSetup();

#endif  // BUTTON_HANDLING_H
