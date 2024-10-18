#include "button_handling.h"
#include "config.h"
#include "motor_state_machine.h"
#include <OneButton.h>

extern MotorStateMachine motorStateMachine;
extern OneButton btn1;
extern OneButton btn2;
extern OneButton ctrl1;
extern OneButton ctrl2;

ButtonState buttonState;  // 按钮状态结构体

void onBtn1Press() {
  buttonState.btn1Pressed = true;
  if (!buttonState.antiPinchTriggered && buttonState.pressureSensorTriggered) {
    motorStateMachine.setState(ACTION_FORWARD_HOLD);
  }
}

void onBtn1Release() {
  buttonState.btn1Pressed = false;
  if (!buttonState.antiPinchTriggered) {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onBtn2Press() {
  buttonState.btn2Pressed = true;
  if (!buttonState.antiPinchTriggered && buttonState.pressureSensorTriggered) {
    motorStateMachine.setState(ACTION_REVERSE_HOLD);
  }
}

void onBtn2Release() {
  buttonState.btn2Pressed = false;
  if (!buttonState.antiPinchTriggered) {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onPressureSensorPress() {
  buttonState.pressureSensorTriggered = true;
  if (!buttonState.antiPinchTriggered) {
    DEBUG_PRINT("Pressure sensor triggered\n");
    if (buttonState.btn1Pressed) {
      motorStateMachine.setState(ACTION_FORWARD_HOLD);
    } else if (buttonState.btn2Pressed) {
      motorStateMachine.setState(ACTION_REVERSE_HOLD);
    }
  }
}

void onPressureSensorRelease() {
  buttonState.pressureSensorTriggered = false;
  if (!buttonState.antiPinchTriggered) {
    DEBUG_PRINT("Pressure sensor released\n");
    if (motorStateMachine.getCurrentState() != ACTION_FORWARD_AUTO && motorStateMachine.getCurrentState() != ACTION_REVERSE_AUTO) {
      motorStateMachine.setState(ACTION_STOP);
    }
  }
}

void onAntiPinchPress() {
  buttonState.antiPinchTriggered = true;
  DEBUG_PRINT("Anti-pinch triggered\n");
  if (motorStateMachine.getCurrentState() == ACTION_REVERSE_STEP || motorStateMachine.getCurrentState() == ACTION_REVERSE_HOLD || motorStateMachine.getCurrentState() == ACTION_REVERSE_AUTO) {
    motorStateMachine.setState(ACTION_FORWARD_STEP);
  } else {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onAntiPinchRelease() {
  buttonState.antiPinchTriggered = false;
  DEBUG_PRINT("Anti-pinch released\n");
}

void bindButtonEvents(OneButton& button, void(onPress)(), void(onRelease)()) {
  button.attachPress(onPress);
  button.attachClick(onRelease);
  button.attachLongPressStop(onRelease);
}

void buttonSetup() {
  bindButtonEvents(btn1, static_cast<void (*)()>(onBtn1Press), static_cast<void (*)()>(onBtn1Release));
  bindButtonEvents(btn2, static_cast<void (*)()>(onBtn2Press), static_cast<void (*)()>(onBtn2Release));
  bindButtonEvents(ctrl1, static_cast<void (*)()>(onPressureSensorPress), static_cast<void (*)()>(onPressureSensorRelease));
  bindButtonEvents(ctrl2, static_cast<void (*)()>(onAntiPinchPress), static_cast<void (*)()>(onAntiPinchRelease));
}