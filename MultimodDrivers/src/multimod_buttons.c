// multimod_buttons.c

#include "../multimod_buttons.h"
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/gpio.h>
#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <inc/hw_types.h>

#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>

#include "../multimod_i2c.h"

static void PCA955_WriteRegister(uint8_t reg, uint8_t data) {
    uint8_t command[2] = {reg, data};
    I2C_WriteMultiple(I2C_A_BASE, PCA9555_I2C_ADDR, command, 2);
}
static uint8_t PCA9555_ReadRegister(uint8_t reg) {
    I2C_WriteSingle(I2C_A_BASE, PCA9555_I2C_ADDR, reg);
    uint8_t data = I2C_ReadSingle(I2C_A_BASE, PCA9555_I2C_ADDR);
    return data;
}

void MultimodButtons_Init(void) {
    PCA955_WriteRegister(6, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
    // int
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
}

void MultimodButtons_IntEnable(void) {
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
}

uint8_t MultimodButtons_Get(void) {
    uint8_t data = PCA9555_ReadRegister(0);
    return data;
}
