// G8RTOS_Scheduler.h
// Date Created: 2023-07-26
// Date Updated: 2023-07-26
// Scheduler / initialization code for G8RTOS

#ifndef G8RTOS_SCHEDULER_H_
#define G8RTOS_SCHEDULER_H_

/************************************Includes***************************************/

#include <stdint.h>

#include "G8RTOS_Structures.h"
#include "../MultimodDrivers/multimod.h"
#include "G8RTOS_Semaphores.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/

/* Status Register with the Thumb-bit Set */
#define THUMBBIT            0x01000000

#define init_R14            0x14141414
#define init_R12            0x12121212
#define init_R3             0x03030303
#define init_R2             0x02020202
#define init_R1             0x01010101
#define init_R0             0x00000000
#define init_R11            0x11111111
#define init_R10            0x10101010
#define init_R9             0x09090909
#define init_R8             0x08080808
#define init_R7             0x07070707
#define init_R6             0x06060606
#define init_R5             0x05050505
#define init_R4             0x04040404

#define MAX_THREADS             12
#define MAX_SEMAPHORES          4
#define OSINT_PRIORITY          7
#define MAX_THREADS_PERIODIC    2

/*************************************Defines***************************************/

/******************************Data Type Definitions********************************/

// Scheduler error typedef
typedef enum {
    NO_ERROR = 0,
    THREAD_LIMIT_REACHED = -1,
    NO_THREADS_SCHEDULED = -2,
    THREADS_INCORRECTLY_ALIVE = -3,
    THREAD_DOES_NOT_EXIST = -4,
    CANNOT_KILL_LAST_THREAD = -5,
    IRQn_INVALID = -6,
    HWI_PRIORITY_INVALID = -7
} sched_ErrCode_t;

/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
/****************************Data Structure Definitions*****************************/

/********************************Public Variables***********************************/

extern tcb_t* CurrentlyRunningThread;
uint32_t SystemTime;

/********************************Public Variables***********************************/

/********************************Public Functions***********************************/

extern void G8RTOS_Start();
extern void PendSV_Handler();

void G8RTOS_Yield(void);
void G8RTOS_Init();
sched_ErrCode_t G8RTOS_Launch();
void G8RTOS_Scheduler();
sched_ErrCode_t G8RTOS_AddThread(threadID_t id, void (*threadToAdd)(void), uint8_t priority, char* string);
void SysTick_Handler();
void G8RTOS_Sleep(uint32_t system_ticks);
sched_ErrCode_t G8RTOS_KillThread(threadID_t id);
sched_ErrCode_t G8RTOS_KillSelf(void);
sched_ErrCode_t G8RTOS_Add_APeriodicEvent(void (*threadHandler)(void), uint32_t interrupt, uint8_t priority);
sched_ErrCode_t G8RTOS_Add_PeriodicEvent(uint32_t execution_time, uint32_t period, void (*threadToAdd)(void));

/********************************Public Functions***********************************/


#endif /* G8RTOS_SCHEDULER_H_ */

 
