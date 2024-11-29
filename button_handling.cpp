#include "button_handling.h"
#include "config.h"
#include "motor_state_machine.h"

extern MotorStateMachine motorStateMachine;
extern MotorStateMachine motorStateMachine2;

Button2 btn1, btn2, btn3, btn4;
Button2 ctrl1, ctrl2;

void pollButtons()
{
  btn1.loop();
  btn2.loop();
  btn3.loop();
  btn4.loop();
  ctrl1.loop();
  ctrl2.loop();
}

void onBtn1Press(Button2 &btn)
{
  DEBUG_PRINT("Button 1 pressed\n");
  if (!ctrl2.isPressed() && ctrl1.isPressed())
  {
    motorStateMachine.setState(ACTION_FORWARD_HOLD);
  }
}

void onBtn1Release(Button2 &btn)
{
  DEBUG_PRINT("Button 1 released\n");
  if (!ctrl2.isPressed())
  {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onBtn2Press(Button2 &btn)
{
  DEBUG_PRINT("Button 2 pressed\n");
  if (!ctrl2.isPressed() && ctrl1.isPressed())
  {
    motorStateMachine.setState(ACTION_REVERSE_HOLD);
  }
}

void onBtn2Release(Button2 &btn)
{
  DEBUG_PRINT("Button 2 released\n");
  if (!ctrl2.isPressed())
  {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onBtn3Press(Button2 &btn)
{
  DEBUG_PRINT("Button 3 pressed\n");
  if (ctrl1.isPressed())
  {
    motorStateMachine2.setState(ACTION_FORWARD_HOLD);
  }
}

void onBtn3Release(Button2 &btn)
{
  DEBUG_PRINT("Button 3 released\n");
  motorStateMachine2.setState(ACTION_STOP);
}

void onBtn4Press(Button2 &btn)
{
  DEBUG_PRINT("Button 4 pressed\n");
  if (ctrl1.isPressed())
  {
    motorStateMachine2.setState(ACTION_REVERSE_HOLD);
  }
}

void onBtn4Release(Button2 &btn)
{
  DEBUG_PRINT("Button 4 released\n");
  motorStateMachine2.setState(ACTION_STOP);
}

void onPressureSensorPress(Button2 &btn)
{
  if (!ctrl2.isPressed())
  {
    DEBUG_PRINT("Pressure sensor triggered\n");
    if (btn1.isPressed())
    {
      motorStateMachine.setState(ACTION_FORWARD_HOLD);
    }
    else if (btn2.isPressed())
    {
      motorStateMachine.setState(ACTION_REVERSE_HOLD);
    }
  }
}

void onPressureSensorRelease(Button2 &btn)
{
  if (!ctrl2.isPressed())
  {
    DEBUG_PRINT("Pressure sensor released\n");
    if (motorStateMachine.getCurrentState() != ACTION_FORWARD_AUTO && motorStateMachine.getCurrentState() != ACTION_REVERSE_AUTO)
    {
      motorStateMachine.setState(ACTION_STOP);
    }
  }
}

void onAntiPinchPress(Button2 &btn)
{
  DEBUG_PRINT("Anti-pinch triggered\n");
  if (motorStateMachine.getCurrentState() == ACTION_REVERSE_STEP || motorStateMachine.getCurrentState() == ACTION_REVERSE_HOLD || motorStateMachine.getCurrentState() == ACTION_REVERSE_AUTO)
  {
    motorStateMachine.setState(ACTION_FORWARD_STEP);
  }
  else
  {
    motorStateMachine.setState(ACTION_STOP);
  }
}

void onAntiPinchRelease(Button2 &btn)
{
  DEBUG_PRINT("Anti-pinch released\n");
}

void buttonSetup()
{
  btn1.begin(BUTTON1_PIN);
  btn1.setPressedHandler(onBtn1Press);
  btn1.setReleasedHandler(onBtn1Release);

  btn2.begin(BUTTON2_PIN);
  btn2.setPressedHandler(onBtn2Press);
  btn2.setReleasedHandler(onBtn2Release);

  btn3.begin(BUTTON3_PIN);
  btn3.setPressedHandler(onBtn3Press);
  btn3.setReleasedHandler(onBtn3Release);

  btn4.begin(BUTTON4_PIN);
  btn4.setPressedHandler(onBtn4Press);
  btn4.setReleasedHandler(onBtn4Release);

  ctrl1.begin(PRESSURE_SENSOR_PIN);
  ctrl1.setPressedHandler(onPressureSensorPress);
  ctrl1.setReleasedHandler(onPressureSensorRelease);

  ctrl2.begin(ANTI_PINCH_PIN);
  ctrl2.setPressedHandler(onAntiPinchPress);
  ctrl2.setReleasedHandler(onAntiPinchRelease);
}