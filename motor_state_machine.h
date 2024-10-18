#ifndef MOTOR_STATE_MACHINE_H
#define MOTOR_STATE_MACHINE_H

#include <Arduino.h>
#include <EEPROM.h>

// 电机初始全局运动方向(用于调整MA MB线序)
enum MotorDirection {
  NORMAL = 1,    // 正常方向
  REVERSED = -1  // 反转方向
};

// 电机状态枚举
enum ActionMode {
  ACTION_STOP = 0,      // 停止
  ACTION_FORWARD_AUTO,  // 自动正转
  ACTION_FORWARD_HOLD,  // 持续正转
  ACTION_FORWARD_STEP,  // 点动正转
  ACTION_REVERSE_AUTO,  // 自动反转
  ACTION_REVERSE_HOLD,  // 持续反转
  ACTION_REVERSE_STEP   // 点动反转
};

// 电机状态机类
class MotorStateMachine {
public:
  MotorStateMachine();  // 构造函数，初始化状态
  void init();
  void setState(ActionMode newState);  // 设置电机状态
  void update();                       // 更新电机状态
  ActionMode getCurrentState() const;  // 获取当前状态
  void reset();                        // 重置状态机

  void toggleDirection();  // 切换方向并保存到EEPROM
  int getDirection();      // 获取当前方向

private:
  ActionMode currentState = ACTION_STOP;  // 当前电机状态
  ActionMode lastState = ACTION_STOP;     // 上一个电机状态
  unsigned long lastActionTime = 0;       // 上次动作的时间戳

  void handleStop();                                            // 处理停止状态
  void handleMotion(bool isForward, bool isAuto, bool isStep);  // 处理运动状态
  void printStateChange(const char* stateName);                 // 打印状态变更信息

  void motorControl(int direction);  // 控制电机方向：1-正转, -1-反转, 0-停止(受到motorDirection影响)

  int motorDirection = NORMAL;     // 默认电机方向
  void loadDirectionFromEEPROM();  // 从EEPROM加载方向状态
  void saveDirectionToEEPROM();    // 保存方向状态到EEPROM
};

#endif  // MOTOR_STATE_MACHINE_H
