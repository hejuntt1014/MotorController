#include "utils.h"
#include "config.h"

// 私有静态变量
static IWDG_HandleTypeDef watchdog;  // 看门狗句柄

// GPIO 配置结构体
struct GPIOConfig {
    uint8_t pin;
    uint8_t mode;
    int initialValue;
};

// GPIO 初始化配置表
const GPIOConfig gpioConfigs[] = {
    {MOTOR_FORWARD_PIN, OUTPUT, LOW},
    {MOTOR_BACKWARD_PIN, OUTPUT, LOW},
    {SPEAKER_PIN, OUTPUT, LOW},
    {PRESSURE_SENSOR_PIN, INPUT_PULLUP, HIGH},
    {ANTI_PINCH_PIN, INPUT_PULLUP, HIGH},
    {BUTTON1_PIN, INPUT_PULLUP, HIGH},
    {BUTTON2_PIN, INPUT_PULLUP, HIGH},
    {RF_DATA_PIN, INPUT_PULLUP, HIGH}
};

// 初始化 GPIO
void initGPIO() {
    for (const auto& config : gpioConfigs) {
        pinMode(config.pin, config.mode);
        digitalWrite(config.pin, config.initialValue);
    }
    DEBUG_PRINT("GPIO initialized\n");
}

// 控制蜂鸣器状态
void speakerControl(bool on) {
    digitalWrite(SPEAKER_PIN, on ? HIGH : LOW);
}

// 获取动作模式名称
const char* getActionModeName(ActionMode mode) {
    switch (mode) {
        case ACTION_STOP: return "Stop";
        case ACTION_FORWARD_AUTO: return "Forward Auto";
        case ACTION_FORWARD_HOLD: return "Forward Hold";
        case ACTION_FORWARD_STEP: return "Forward Step";
        case ACTION_REVERSE_AUTO: return "Reverse Auto";
        case ACTION_REVERSE_HOLD: return "Reverse Hold";
        case ACTION_REVERSE_STEP: return "Reverse Step";
        default: return "Unknown";
    }
}

// 初始化看门狗，并增加错误处理和重试机制
void initWatchdog() {
    __HAL_RCC_LSI_ENABLE();
    watchdog.Instance = IWDG;
    watchdog.Init.Prescaler = IWDG_PRESCALER_64;
    watchdog.Init.Reload = 512;

    for (int i = 0; i < 3; ++i) {
        if (HAL_IWDG_Init(&watchdog) == HAL_OK) {
            DEBUG_PRINT("IWDG initialized successfully\n");
            return;
        }
        delay(100);
    }

    // DEBUG_PRINT("IWDG initialization failed, system restarting...\n");
    // NVIC_SystemReset();  // 系统重启
}

// 看门狗刷新逻辑
void refreshWatchdog() {
    HAL_IWDG_Refresh(&watchdog);
}

// 判断 unsigned long 类型的值是否有效
bool isValidUnsignedLong(unsigned long value) {
    return value != 0 && value != 0xFFFFFFFF;
}
