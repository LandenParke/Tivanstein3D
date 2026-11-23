// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_LaunchpadButtons.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// LaunchpadButtons_Init
// Initializes the GPIO port & pins necessary for the button switches on the
// launchpad.
// Return: void
void LaunchpadButtons_Init() {
    // Enable port F for switches
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // write unlock key for GPIO lock and commit changes
    GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);

    // Use SW1 & SW2, configure as inputs.
    GPIODirModeSet(
        GPIO_PORTF_BASE,
        GPIO_PIN_4 | GPIO_PIN_0,
        GPIO_DIR_MODE_IN
    );
    GPIOPadConfigSet(
        GPIO_PORTF_BASE,
        GPIO_PIN_4 | GPIO_PIN_0,
        GPIO_STRENGTH_2MA,
        GPIO_PIN_TYPE_STD_WPU
    );

    // Pay attention to the pin connected to SW2!!! PF0 is a locked pin.
}

// LaunchpadButtons_ReadSW1
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW1() {
    // GPIOPinRead() may be of use
    uint8_t value = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

    // your code here
    return !value;

}

// LaunchpadButtons_ReadSW2
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW2() {
    // GPIOPinRead() may be of use
    uint8_t value = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

    // your code here
    return !value;
}



/********************************Public Functions***********************************/
