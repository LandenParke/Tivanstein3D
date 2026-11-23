// G8RTOS_Threads.c
// Date Created: 2023-07-25
// Date Updated: 2025-07-27
// Defines for thread functions.

/************************************Includes***************************************/

#include "./threads.h"

#include "./MultimodDrivers/multimod.h"
#include "./MiscFunctions/Shapes/inc/cube.h"
#include "./MiscFunctions/LinAlg/inc/linalg.h"
#include "./MiscFunctions/LinAlg/inc/quaternions.h"
#include "./MiscFunctions/LinAlg/inc/vect3d.h"
#include "./G8RTOS/G8RTOS_IPC.h"
#include "./G8RTOS/G8RTOS_Semaphores.h"
#include "./G8RTOS/G8RTOS_Scheduler.h"

#define STB_BITWISE_IMPLEMENTATION
#include "./stb_bitwise.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/************************************Includes***************************************/

/*************************************Defines***************************************/

// Change this to change the number of points that make up each line of a cube.
// Note that if you set this too high you will have a stack overflow!
// sizeof(float) * num_lines * (Num_Interpolated_Points + 2) = ?
#define Num_Interpolated_Points 3


/*************************************Defines***************************************/

/*********************************Global Variables**********************************/
/*********************************Global Variables**********************************/

/*************************************Threads***************************************/

void Idle_Thread(void) {
    // your code
    for (;;);
}

void CamMove_Thread(void) {
    uint32_t result;
    int16_t joystickX;
    int16_t joystickY;
    float joystickX_norm;
    float joystickY_norm;

    world_camera_pos.x = 0;
    world_camera_pos.y = 0;
    world_camera_pos.z = 60;

    /*
    Quat_t rot_quat, temp;

    float cr, sr, cp, sp, cy, sy;
    float mag;
    */

    while(1) {
        result = G8RTOS_ReadFIFO(JOYSTICK_FIFO);

        // TODO: Normalize joystick input
        joystickX = ((int16_t)((result & 0xFFFF0000) >> 16) - 2000) / 500;
        joystickY = ((int16_t)((result & 0x0000FFFF) >> 0) - 2000) / 500;
        // UARTprintf("x: %d\ny:%d\n", joystickX, joystickY);
        uint32_t status = StartCriticalSection();
        joystickX_norm = (float)(joystickX);
        joystickY_norm = (float)(joystickY);
        EndCriticalSection(status);
        world_camera_pos.x -= joystickX_norm;

        if (joystick_y) {
            world_camera_pos.y += joystickY_norm;
        } else {
            world_camera_pos.z -= joystickY_norm;
        }

        G8RTOS_Sleep(10);
        G8RTOS_Yield();

    }
}

void Read_Buttons() {
    while(1) {
        // your code goes here
        G8RTOS_WaitSemaphore(PCA9555_Semaphore);
        G8RTOS_Sleep(10);
        G8RTOS_Yield();
        G8RTOS_WaitSemaphore(I2C_Semaphore);
        uint8_t buttons = MultimodButtons_Get();
        G8RTOS_SignalSemaphore(I2C_Semaphore);
        if (stb_bit_cleared(buttons, PCA9555_SW1_bp)) {
            uint8_t x, y, z;
            x = (uint16_t)(rand() % 200);
            y = (uint16_t)(rand() % 200);    
            z = (uint16_t)(rand() % 100);
            uint32_t packet = ( (((uint32_t)(x) & 0xFFF) << 20) |
                                (((uint32_t)(y) & 0xFFF) << 8) |
                                (((uint32_t)(z) & 0xFF) << 0)
                                );
            G8RTOS_WriteFIFO(SPAWNCOOR_FIFO, packet);
            sched_ErrCode_t err = G8RTOS_AddThread(47 + num_cubes, Cube_Thread, 99, "cube");
            if (err == NO_ERROR) num_cubes++;
        } 
        if (stb_bit_cleared(buttons, PCA9555_SW2_bp)) {
            if (kill_cube == 0) {
                kill_cube = 1;
                num_cubes--;
            }
        }
        GPIOIntClear(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
        GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
    }
}

/********************************Periodic Threads***********************************/

void Get_Joystick(void) {
	// your code
    uint32_t x_then_y[2];
    JOYSTICK_GetXY(x_then_y);
    uint32_t result = ((x_then_y[0] & 0xFFFF) << 16) | ((x_then_y[1] & 0xFFFF) << 0);
    G8RTOS_WriteFIFO_Mutexless(JOYSTICK_FIFO, result);
}

/********************************Periodic Threads***********************************/


/*******************************Aperiodic Threads***********************************/

void GPIOE_Handler() {
    // your code
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
    GPIOIntDisable(GPIO_PORTE_BASE, GPIO_INT_PIN_4);
    G8RTOS_SignalSemaphore(PCA9555_Semaphore);
}

/*******************************Aperiodic Threads***********************************/
