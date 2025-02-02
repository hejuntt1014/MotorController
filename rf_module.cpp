#include "rf_module.h"
#include "config.h"
#include "utils.h"
#include "beeper.h"
#include "motor_state_machine.h"
#include <EEPROM.h>

extern MotorStateMachine motorStateMachine;
extern Beeper beeper;

// 初始化RF模块
void RFModule::init() {
  rcSwitch.enableReceive(digitalPinToInterrupt(RF_DATA_PIN));
  loadRFCode();
  configStartTime = millis();
  DEBUG_PRINT("RF module initialized, entering config mode\n");
}

// 处理RF配置
void RFModule::handleConfig() {
  if (rcSwitch.available()) {
    unsigned long code = rcSwitch.getReceivedValue();
    if (code != 0) {
      DEBUG_PRINT("Received code in config mode: %lu\n", code);
      if (code == lastReceivedRFCode) {
        if (!isPressing) {
          pressStartTime = millis();
          isPressing = true;
        } else if (millis() - pressStartTime >= RF_LONG_PRESS_DURATION) {
          saveRFCode(code);
          inConfigMode = false;
          DEBUG_PRINT("RF code bound, entering normal mode\n");
        }
      } else {
        isPressing = false;
      }
      lastReceivedRFCode = code;
    }
    rcSwitch.resetAvailable();
  }

  if (millis() - configStartTime > RF_CONFIG_WINDOW) {
    inConfigMode = false;
    DEBUG_PRINT("Config window expired, entering normal mode\n");
  }
}

// 处理RF正常模式
void RFModule::handleNormalMode() {
  if (rcSwitch.available()) {
    unsigned long code = rcSwitch.getReceivedValue();

    unsigned long currentTime = millis();

    // 检查两次RF指令之间的时间间隔是否足够
    if (code == lastReceivedRFCode && (currentTime - lastRFCodeTime < RF_DEBOUNCE_INTERVAL)) {
      DEBUG_PRINT("Ignored RF code (debounce): %lu\n", code);
      rcSwitch.resetAvailable();  // 重置RF状态
      return;                     // 忽略此次指令
    }

    // 更新最后的按键代码和时间
    lastReceivedRFCode = code;
    lastRFCodeTime = currentTime;

    DEBUG_PRINT("Received code in normal mode: %lu\n", code);

    if (isValidRFCode(storedRFCode)) {
      if (code == storedRFCode) {
        motorStateMachine.setState(ACTION_STOP);
      } else if (code == storedRFCode + RF_FORWARD_CODE_OFFSET) {
        motorStateMachine.setState(ACTION_FORWARD_AUTO);
      } else if (code == storedRFCode - RF_REVERSE_CODE_OFFSET) {
        motorStateMachine.setState(ACTION_REVERSE_AUTO);
      } else if (code == storedRFCode - RF_DIRECTION_TOGGLE_CODE) {
        motorStateMachine.toggleDirection();
        DEBUG_PRINT("RF code detected: Direction toggled. New Direction: %d\n", motorStateMachine.getDirection());
      } else {
        DEBUG_PRINT("Unknown RF code: %lu\n", code);
      }
    } else {
      DEBUG_PRINT("Invalid stored RF code, ignoring\n");
    }
    rcSwitch.resetAvailable();
  }
}

// 判断是否处于配置模式
bool RFModule::isInConfigMode() const {
  return inConfigMode;
}

// 保存RF代码
void RFModule::saveRFCode(unsigned long code) {
  if (!isValidRFCode(code)) {
    DEBUG_PRINT("Invalid RF code: %lu\n", code);

    // 无效的RF代码，蜂鸣器响4下
    beeper.startBeep(4, SPEAKER_DURATION, SPEAKER_INTERVA);
    return;
  }

  if (code == storedRFCode) {
    DEBUG_PRINT("RF code already bound: %lu\n", code);

    // RF代码重复，蜂鸣器响2下
    beeper.startBeep(2, SPEAKER_DURATION, SPEAKER_INTERVA);
    return;
  }

  // 代码有效且不重复，保存到EEPROM
  EEPROM.put(RF_EEPROM_ADDRESS, code);
  storedRFCode = code;
  DEBUG_PRINT("RF code saved to EEPROM: %lu\n", code);

  // 保存成功，蜂鸣器响3下
  beeper.startBeep(3, SPEAKER_DURATION, SPEAKER_INTERVA);
}


// 加载RF代码
void RFModule::loadRFCode() {
  EEPROM.get(RF_EEPROM_ADDRESS, storedRFCode);
  if (isValidRFCode(storedRFCode)) {
    DEBUG_PRINT("Loaded RF code from EEPROM: %lu\n", storedRFCode);
  } else {
    storedRFCode = 0;
    DEBUG_PRINT("No valid RF code found in EEPROM\n");
  }
}

// 判断RF代码是否有效
bool RFModule::isValidRFCode(unsigned long code) {
  return isValidUnsignedLong(code);
}
