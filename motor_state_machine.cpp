#include "motor_state_machine.h"
#include "config.h"
#include "utils.h"
#include "beeper.h"

extern Beeper beeper;

// 构造函数，初始化状态机
MotorStateMachine::MotorStateMachine(int forwardPin, int backwardPin, int directionEEPROMAddr)
  : forwardPin(forwardPin), backwardPin(backwardPin), directionEEPROMAddr(directionEEPROMAddr) {
  currentState = ACTION_STOP;
  lastState = ACTION_STOP;
  lastActionTime = millis();
}

// 初始化状态机并加载方向状态
void MotorStateMachine::init() {
  loadDirectionFromEEPROM();
}

// 从EEPROM加载电机方向状态
void MotorStateMachine::loadDirectionFromEEPROM() {
  // 从 EEPROM 中读取方向状态到 motorDirection
  EEPROM.get(directionEEPROMAddr, motorDirection);
  DEBUG_PRINT("Loaded Direction from EEPROM: %lu\n", motorDirection);

  // 检查是否为有效方向，否则重置为 NORMAL
  if (!(isValidUnsignedLong(motorDirection) && (motorDirection == NORMAL || motorDirection == REVERSED))) {
    DEBUG_PRINT("Invalid direction. Resetting to NORMAL.\n");
    motorDirection = NORMAL;
    saveDirectionToEEPROM();  // 保存默认值到 EEPROM
  }
}

// 保存当前方向到EEPROM
void MotorStateMachine::saveDirectionToEEPROM() {
  EEPROM.put(directionEEPROMAddr, motorDirection);
}

// 切换电机方向并保存到EEPROM
void MotorStateMachine::toggleDirection() {
  motorDirection = (motorDirection == NORMAL) ? REVERSED : NORMAL;
  saveDirectionToEEPROM();
  beeper.startBeep(2, SPEAKER_DURATION * 2, SPEAKER_INTERVA);
}

// 获取当前方向
int MotorStateMachine::getDirection() {
  return motorDirection;
}

// 在 motorControl 中使用当前方向
void MotorStateMachine::motorControl(int direction) {
  // 获取方向并计算最终方向  
  int finalDirection = direction * motorDirection;

  switch (finalDirection) {
    case 1:
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, HIGH);
      break;
    case -1:
      digitalWrite(forwardPin, HIGH);
      digitalWrite(backwardPin, LOW);
      break;
    default:
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, LOW);
      break;
  }
}

// 设置电机状态
void MotorStateMachine::setState(ActionMode newState) {
  if (newState != currentState) {
    lastState = currentState;
    currentState = newState;
    lastActionTime = millis();
    beeper.startBeep(SPEAKER_DURATION);
    printStateChange(getActionModeName(currentState));
  }
}

// 更新电机状态
void MotorStateMachine::update() {
  switch (currentState) {
    case ACTION_STOP:
      handleStop();
      break;
    case ACTION_FORWARD_AUTO:
    case ACTION_FORWARD_HOLD:
    case ACTION_FORWARD_STEP:
      handleMotion(true, currentState == ACTION_FORWARD_AUTO, currentState == ACTION_FORWARD_STEP);
      break;
    case ACTION_REVERSE_AUTO:
    case ACTION_REVERSE_HOLD:
    case ACTION_REVERSE_STEP:
      handleMotion(false, currentState == ACTION_REVERSE_AUTO, currentState == ACTION_REVERSE_STEP);
      break;
  }
}

// 获取当前状态
ActionMode MotorStateMachine::getCurrentState() const {
  return currentState;
}

// 重置状态机
void MotorStateMachine::reset() {
  currentState = ACTION_STOP;
  lastState = ACTION_STOP;
  lastActionTime = millis();
  DEBUG_PRINT("Motor state machine reset\n");
}

// 处理停止状态
void MotorStateMachine::handleStop() {
  motorControl(0);
}

// 处理电机运动
void MotorStateMachine::handleMotion(bool isForward, bool isAuto, bool isStep) {
  motorControl(isForward ? 1 : -1);
  unsigned long duration = isStep ? STEP_DURATION : (isAuto ? AUTO_DURATION : MAX_DURATION);

  if (millis() - lastActionTime > duration) {
    setState(ACTION_STOP);
  }
}

// 打印状态变更信息
void MotorStateMachine::printStateChange(const char *stateName) {
  DEBUG_PRINT("Motor state changed to: %s\n", stateName);
}
