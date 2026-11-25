#ifndef MULTIMOD_H_
#define MULTIMOD_H_

#include "multimod_uart.h"
#include "multimod_i2c.h"
#include "multimod_BMI160.h"
#include "multimod_OPT3001.h"
#include "multimod_BME280.h"
#include "multimod_LaunchpadButtons.h"
#include "multimod_LaunchpadLED.h"
#include "multimod_buttons.h"
#include "multimod_joystick.h"
#include "multimod_ST7789.h"

static void Multimod_Init() {
//    BMI160_Init();
//    OPT3001_Init();
    I2C_Init(I2C_A_BASE);
    UART_Init();
//    BMI160_Init();
//    OPT3001_Init();
    LaunchpadLED_Init();
    MultimodButtons_Init();
    MultimodButtons_IntEnable();
    JOYSTICK_Init();
    JOYSTICK_IntEnable();
    ST7789_Init();
    SysCtlDelay(300);

    ST7789_Fill(ST7789_GREEN);
    SysCtlDelay(300);
    ST7789_Fill(ST7789_BLACK);
    SysCtlDelay(300);

    //BME2028_Init();
}

#endif /* MULTIMOD_H_ */
