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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/************************************Includes***************************************/

/*************************************Defines***************************************/
/*************************************Defines***************************************/

/*********************************Global Variables**********************************/
/*********************************Global Variables**********************************/

/*************************************Threads***************************************/

void Idle_Thread(void) {
    // your code
    for (;;);
}

/********************************Periodic Threads***********************************/
/********************************Periodic Threads***********************************/


/*******************************Aperiodic Threads***********************************/
/*******************************Aperiodic Threads***********************************/
