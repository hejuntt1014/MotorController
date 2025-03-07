#include "motor_state_machine.h"
#include "config.h"
#include "utils.h"
#include "beeper.h"

extern Beeper beeper;

// 构造函数，初始化状态机
MotorStateMachine::MotorStateMachine(int forwardPin, int backwardPin, int directionEEPROMAddr, int motorIndex)
    : forwardPin(forwardPin), backwardPin(backwardPin), directionEEPROMAddr(directionEEPROMAddr), motorIndex(motorIndex)
{
  currentState = ACTION_STOP;
  lastState = ACTION_STOP;
  lastActionTime = millis();
  
  // 根据电机索引设置固定延迟时间(第一个电机0ms, 第二个电机50ms)
  randomDelay = motorIndex * 50;
}

// 初始化状态机并加载方向状态
void MotorStateMachine::init()
{
  loadDirectionFromEEPROM();
  
  // 确保初始状态为停止
  analogWrite(forwardPin, 0);
  analogWrite(backwardPin, 0);
}

// 从EEPROM加载电机方向状态
void MotorStateMachine::loadDirectionFromEEPROM()
{
  // 从 EEPROM 中读取方向状态到 motorDirection
  EEPROM.get(directionEEPROMAddr, motorDirection);
  DEBUG_PRINT("Loaded Direction from EEPROM: %lu\n", motorDirection);

  // 检查是否为有效方向，否则重置为 NORMAL
  if (!(isValidUnsignedLong(motorDirection) && (motorDirection == NORMAL || motorDirection == REVERSED)))
  {
    DEBUG_PRINT("Invalid direction. Resetting to NORMAL.\n");
    motorDirection = NORMAL;
    saveDirectionToEEPROM(); // 保存默认值到 EEPROM
  }
}

// 保存当前方向到EEPROM
void MotorStateMachine::saveDirectionToEEPROM()
{
  EEPROM.put(directionEEPROMAddr, motorDirection);
}

// 切换电机方向并保存到EEPROM
void MotorStateMachine::toggleDirection()
{
  motorDirection = (motorDirection == NORMAL) ? REVERSED : NORMAL;
  saveDirectionToEEPROM();
  beeper.startBeep(2, SPEAKER_DURATION * 2, SPEAKER_INTERVA);
}

// 获取当前方向
int MotorStateMachine::getDirection()
{
  return motorDirection;
}

// 在 motorControl 中使用当前方向和PWM控制
void MotorStateMachine::motorControl(int direction)
{
  // 获取方向并计算最终方向
  int finalDirection = direction * motorDirection;
  
  // 更新PWM值
  updatePWM();

  switch (finalDirection)
  {

  // case 1:
  //   digitalWrite(forwardPin, HIGH);
  //   digitalWrite(backwardPin, LOW);
  //   break;
  // case -1:
  //   digitalWrite(forwardPin, LOW);
  //   digitalWrite(backwardPin, HIGH);
  //   break;

  case 1:  // 正转
    // 输出
    // DEBUG_PRINT("%d\n", currentPWM);
    analogWrite(forwardPin, currentPWM);
    analogWrite(backwardPin, 0);
    break;
  case -1:  // 反转
    // DEBUG_PRINT("%d\n", currentPWM);
    analogWrite(forwardPin, 0);
    analogWrite(backwardPin, currentPWM);
    break;
  case 0:  // 停止
    analogWrite(forwardPin, 0);
    analogWrite(backwardPin, 0);
    resetPWM();  // 重置PWM值
    break;
  default:  // 刹车
    analogWrite(forwardPin, 255);
    analogWrite(backwardPin, 255);
    resetPWM();  // 重置PWM值
    break;
  }
}

// 更新PWM值
void MotorStateMachine::updatePWM()
{
  unsigned long currentTime = millis();
  
  // 检查是否需要更新PWM
  if (currentTime - lastPWMUpdateTime >= PWM_UPDATE_INTERVAL)
  {
    lastPWMUpdateTime = currentTime;
    
    // 如果当前PWM小于目标PWM,则逐步增加
    if (currentPWM < targetPWM)
    {
      // 记录第一次PWM更新的时间
      if (currentPWM == 0) {
        startPWMTime = currentTime;
      }
      
      // 先计算新的PWM值
      uint8_t newPWM = currentPWM + PWM_STEP;
      // 确保不超过目标值
      currentPWM = (newPWM > targetPWM) ? targetPWM : newPWM;
      
      // 如果达到目标PWM，输出总时间
      if (currentPWM == targetPWM) {
        DEBUG_PRINT("PWM从0增加到%d花费时间: %lu ms\n", targetPWM, currentTime - startPWMTime);
      }
    }
  }
}

// 重置PWM值
void MotorStateMachine::resetPWM()
{
  currentPWM = 0;
  lastPWMUpdateTime = millis();
}

// 设置电机状态
void MotorStateMachine::setState(ActionMode newState)
{
  if (newState != currentState)
  {
    lastState = currentState;
    currentState = newState;
    lastActionTime = millis();
    
    // 如果是从停止状态切换到运动状态，或者改变运动方向，重置PWM值
    if (lastState == ACTION_STOP || 
        (isForwardState(currentState) && isReverseState(lastState)) ||
        (isReverseState(currentState) && isForwardState(lastState)))
    {
      resetPWM();
    }
    
    beeper.startBeep(SPEAKER_DURATION);
    printStateChange(getActionModeName(currentState));
  }
}

// 判断是否为前进状态
bool MotorStateMachine::isForwardState(ActionMode state) const
{
  return state == ACTION_FORWARD_AUTO || 
         state == ACTION_FORWARD_HOLD || 
         state == ACTION_FORWARD_STEP;
}

// 判断是否为后退状态
bool MotorStateMachine::isReverseState(ActionMode state) const
{
  return state == ACTION_REVERSE_AUTO || 
         state == ACTION_REVERSE_HOLD || 
         state == ACTION_REVERSE_STEP;
}

// 更新电机状态
void MotorStateMachine::update()
{
  switch (currentState)
  {
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
ActionMode MotorStateMachine::getCurrentState() const
{
  return currentState;
}

// 重置状态机
void MotorStateMachine::reset()
{
  currentState = ACTION_STOP;
  lastState = ACTION_STOP;
  lastActionTime = millis();
  DEBUG_PRINT("Motor state machine reset\n");
}

// 处理停止状态
void MotorStateMachine::handleStop()
{
  motorControl(0);
  // if (millis() - lastActionTime < 50)
  // {
  //   motorControl(0);
  // }
  // else if (millis() - lastActionTime < 150)
  // {
  //   motorControl(-2);  
  // }
  // else
  // {
  //   motorControl(0);
  // } 
}

// 处理电机运动
void MotorStateMachine::handleMotion(bool isForward, bool isAuto, bool isStep)
{
  // 检查是否需要电机刹车时间
  //if (lastState != ACTION_STOP && (millis() - lastActionTime < MOTOR_BRAKE_TIME))
  if (millis() - lastActionTime <    + randomDelay)
  {
    handleStop();  // 电机刹车,防止反向电动势
    return;
  }

  motorControl(isForward ? 1 : -1);
  unsigned long duration = isStep ? STEP_DURATION : (isAuto ? AUTO_DURATION : MAX_DURATION);

  if (millis() - lastActionTime > duration)
  {
    setState(ACTION_STOP);
  }
}

// 打印状态变更信息
void MotorStateMachine::printStateChange(const char *stateName)
{
  DEBUG_PRINT("Motor random delay: %lu ms\n", randomDelay);
  DEBUG_PRINT("Motor state changed to: %s\n", stateName);
}
