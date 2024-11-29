#include "config.h"
#include "motor_state_machine.h"
#include "rf_module.h"
#include "button_handling.h"
#include "utils.h"
#include "beeper.h"
#include <OneButton.h>

/**
  @brief 电机控制器简易测试程序
  @details 控制器支持24~36v电压输入，控制直流电机正反转，4个外部触发按钮
  请在工具栏配置芯片 5V供电和16Mhz主频
  RA,RB为正反转控制引脚，通过ULN2001D驱动输出，再由两个继电器隔离输出；
  SPK为12V有源蜂鸣器引脚，通过ULN2001D驱动输出，有22k上拉电阻和100欧限流电阻；

  CTRL_1 短接C1+,C1-可触发，由光耦隔离。同时支持12V~20V外部输入接入C1-触发；
  CTRL_2 短接C2+,C2-可触发，由光耦隔离。同时支持12V~20V外部输入接入C2-触发；
  BTN_1 短接B1,VCC可触发，由光耦隔离。同时支持24~36V外部输入接入B1触发；
  BTN_2 短接B2,VCC可触发，由光耦隔离。同时支持24~36V外部输入接入B2触发；
  过压可能会造成电路损坏，请注意。

  RF_DATA 预设为315Mhz和433Mhz射频模块接口，无外部上下拉电阻，需要上下拉功能请在单片机内配置。

  电机控制器(电动沙发座椅控制器)支持以下功能：
  1.BTN_1，BTN_2为电机的手动控制按键，正转为电动沙发座垫展开，反转为电动沙发座垫收回；
  2.CTRL_1为座垫压力传感器，仅在CTRL_1按下时，BTN_1和BTN_2按下时电机持续转动，松开停止；
  3.CTRL_2为防夹脚保护，如果CTRL_2被触发,正转时自动停止，反转时点动反转后停止；
  4.支持RF射频遥控器，开机的配置模式下，长按遥控器按键中间按键x秒，可绑定遥控器编码，绑定后进入正常模式；
  5.四键遥控器最后一个按键用于反转电机方向

  @version v20241011.01-beta
*/

// 全局对象
RFModule rfModule;
Beeper beeper;
MotorStateMachine motorStateMachine(MOTOR_FORWARD_PIN, MOTOR_BACKWARD_PIN, DIRECTION_EEPROM_ADDR);
MotorStateMachine motorStateMachine2(MOTOR2_FORWARD_PIN, MOTOR2_BACKWARD_PIN, DIRECTION_EEPROM_ADDR2);
MotorStateMachines motors = {&motorStateMachine, &motorStateMachine2 /* &motor2, ... */}; // 创建电机状态机向量

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  initGPIO();
  buttonSetup();
  rfModule.init();
  initWatchdog();

  for (auto &motor : motors)
  {
    motor->init();
  }

  DEBUG_PRINT("System initialized\n");
}

void loop()
{
  refreshWatchdog(); // 喂狗

  pollButtons();
  beeper.update(); // 蜂鸣器

  for (auto &motor : motors)
  {
    motor->update();
  }

  if (rfModule.isInMatchMode())
  {
    rfModule.handleMatch();
  }
  else if (rfModule.isInConfigMode())
  {
    rfModule.handleConfig();
  }
  else
  {
    rfModule.handleNormalMode();
  }
}
