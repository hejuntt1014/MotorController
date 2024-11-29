// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// 调试模式配置
#define DEBUG_MODE 1 // 设置为0可以在发布时禁用调试输出

// 配置文件版本
constexpr char CONFIG_VERSION[] = "1.0.0";

// 电机控制相关参数
constexpr unsigned long MAX_DURATION = 0xFFFFFFFF;

// 引脚定义
constexpr uint8_t MOTOR_FORWARD_PIN = PB_1;   // 电机正转引脚(MA1)
constexpr uint8_t MOTOR_BACKWARD_PIN = PB_2;  // 电机反转引脚(MB1)
constexpr uint8_t MOTOR2_FORWARD_PIN = PA_1;  // 电机正转引脚(MA2)
constexpr uint8_t MOTOR2_BACKWARD_PIN = PA_0; // 电机反转引脚(MB2)
constexpr uint8_t SPEAKER_PIN = PA_6;         // 蜂鸣器引脚(SPK)
constexpr uint8_t BUTTON1_PIN = PA_13;        // 按钮1引脚(BTN0_1)
constexpr uint8_t BUTTON2_PIN = PA_14;        // 按钮2引脚(BTN0_0)
constexpr uint8_t BUTTON3_PIN = PA_7;         // 按钮3引脚(BTN1_1)
constexpr uint8_t BUTTON4_PIN = PA_4;         // 按钮4引脚(BTN1_0)
constexpr uint8_t PRESSURE_SENSOR_PIN = PF_0; // 压力传感器引脚(CTRL_2)
constexpr uint8_t ANTI_PINCH_PIN = PF_1;      // 防夹保护引脚(CTRL_3)
constexpr uint8_t RF_DATA_PIN = PB_0;         // 射频数据引脚(RF_DATA)
constexpr uint8_t CTRL_1_PIN = PB_3;

// 时间常量
constexpr unsigned long SPEAKER_DURATION = 200;       // 蜂鸣器持续时间 (ms)
constexpr unsigned long SPEAKER_INTERVA = 100;        // 蜂鸣器间隔时间 (ms)
constexpr unsigned long STEP_DURATION = 1000;         // 点动持续时间 (ms)
constexpr unsigned long AUTO_DURATION = 14000;        // 自动模式持续时间 (ms)
constexpr unsigned long RF_CONFIG_WINDOW = 3000;      // RF配置窗口时间 (ms)
constexpr unsigned long RF_LONG_PRESS_DURATION = 500; // RF长按持续时间 (ms)
constexpr unsigned long RF_DEBOUNCE_INTERVAL = 500;   // 正常模式下遥控器按键防抖间隔 (ms)

// EEPROM存储地址
constexpr unsigned long RF_EEPROM_ADDRESS = 0;      // RF代码存储地址
constexpr unsigned long DIRECTION_EEPROM_ADDR = 4;  // 电机1运动方向
constexpr unsigned long DIRECTION_EEPROM_ADDR2 = 8; // 电机2运动方向

// RF编码偏移量
constexpr unsigned long RF_FORWARD_CODE_OFFSET = 4;     // 遥控器前进编码偏移
constexpr unsigned long RF_REVERSE_CODE_OFFSET = 2;     // 遥控器后退编码偏移
constexpr unsigned long RF_CONFIG_MODE_CODE_OFFSET = 3; // 遥控器进入配置模式

#if DEBUG_MODE
#define DEBUG_PRINT(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#endif // CONFIG_H
