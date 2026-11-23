// Lab 4, uP2 Fall 2025
// test
// Created: 2023-07-31
// Updated: 2025-07-09
// Lab 3 is intended to introduce you to RTOS concepts. In this, you will
// - configure the systick function
// - write asm functions for context switching
// - write semaphore functions
// - write scheduler functions to add threads / run scheduling algorithms
// - write critical section assembly functions

/************************************Includes***************************************/

#include <stdbool.h>

#include "./G8RTOS/G8RTOS.h"
#include "./MultimodDrivers/multimod.h"

#include "./threads.h"
#include "./G8RTOS/G8RTOS_IPC.h"
#include "driverlib/fpu.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/
/*************************************Defines***************************************/

/********************************Public Variables***********************************/

/********************************Public Variables***********************************/

/********************************Public Functions***********************************/
/********************************Public Functions***********************************/

/************************************MAIN*******************************************/

// Be sure to add in your source files from previous labs into "MultimodDrivers/src/"!
// If you made any modifications to the corresponding header files, be sure to update
// those, too.
int main(void) {

    // sysclock 80mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlDelay(10);

    // Initializes the necessary peripherals.
    Multimod_Init();

    // Add threads, initialize semaphores here!

    // launch your RTOS!
        // your code here
    G8RTOS_Init();

    FPUStackingDisable();

    G8RTOS_Launch();

    // spin - the RTOS will take over now
    while (1);

}

/************************************MAIN*******************************************/
