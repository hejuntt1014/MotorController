// button_handling.h
#ifndef BUTTON_HANDLING_H
#define BUTTON_HANDLING_H

#include <Arduino.h>
#include <Button2.h>

// 外部声明按钮对象
extern Button2 btn1;
extern Button2 btn2;
extern Button2 btn3;
extern Button2 btn4;
extern Button2 ctrl1;
extern Button2 ctrl2;

void onBtn1Press(Button2 &btn);             // 按钮1按下事件处理函数
void onBtn1Release(Button2 &btn);           // 按钮1释放事件处理函数
void onBtn2Press(Button2 &btn);             // 按钮2按下事件处理函数
void onBtn2Release(Button2 &btn);           // 按钮2释放事件处理函数
void onBtn3Press(Button2 &btn);             // 按钮3按下事件处理函数
void onBtn3Release(Button2 &btn);           // 按钮3释放事件处理函数
void onBtn4Press(Button2 &btn);             // 按钮4按下事件处理函数
void onBtn4Release(Button2 &btn);           // 按钮4释放事件处理函数
void onPressureSensorPress(Button2 &btn);   // 压力传感器按下事件处理函数
void onPressureSensorRelease(Button2 &btn); // 压力传感器释放事件处理函数
void onAntiPinchPress(Button2 &btn);        // 防夹传感器按下事件处理函数
void onAntiPinchRelease(Button2 &btn);      // 防夹传感器释放事件处理函数

void buttonSetup(); // 初始化按钮设置
void pollButtons(); // 轮询所有按钮状态

#endif // BUTTON_HANDLING_H