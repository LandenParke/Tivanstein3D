// G8RTOS_Semaphores.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for semaphore functions

/************************************Includes***************************************/

#include <stdint.h>
#include "../G8RTOS_Semaphores.h"
#include "../G8RTOS_Scheduler.h"
#include "../G8RTOS_CriticalSection.h"

/************************************Includes***************************************/

/******************************Data Type Definitions********************************/
/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
/****************************Data Structure Definitions*****************************/

/***********************************Externs*****************************************/

extern tcb_t* CurrentlyRunningThread;
extern tcb_t tcb_array[MAX_THREADS];

/***********************************Externs*****************************************/

/********************************Public Variables***********************************/

// Semaphores
semaphore_t UART_Semaphore_original;
semaphore_t* UART_Semaphore = &UART_Semaphore_original;
semaphore_t I2C_Semaphore_original;
semaphore_t* I2C_Semaphore = &I2C_Semaphore_original;
semaphore_t SPI_Semaphore_original;
semaphore_t *SPI_Semaphore = &SPI_Semaphore_original;
semaphore_t Joystick_Semaphore_original;
semaphore_t* Joystick_Semaphore = &Joystick_Semaphore_original;
semaphore_t sem_KillCube_original;
semaphore_t* sem_KillCube = &sem_KillCube_original;
semaphore_t PCA9555_Semaphore_original;
semaphore_t *PCA9555_Semaphore = &PCA9555_Semaphore_original;

/********************************Public Variables***********************************/

/*******************************Private Variables***********************************/
/*******************************Private Variables***********************************/

/*******************************Private Functions***********************************/

// G8RTOS_Block
// Block a thread with its attached semaphore
static void G8RTOS_Block(semaphore_t *s) {
    CurrentlyRunningThread->blocked = s; 
}

// G8RTOS_Unblock
// Unblocks highest priority thread, handles same priorities through last_ran
// Return: bool (true = thread unblocked, false = no unblock)
bool G8RTOS_Unblock(semaphore_t *s) {
    tcb_t* thread = NULL;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (tcb_array[i].alive && tcb_array[i].blocked == s) {
            if (thread == NULL || tcb_array[i].priority < thread->priority) {
                thread = &tcb_array[i];
            } else if (tcb_array[i].priority == thread->priority && tcb_array[i].last_ran < thread->last_ran) {
                thread = &tcb_array[i];
            }   
        }
    }
    if (thread == NULL) return false;
    thread->blocked = NULL;
    return true;
}

/********************************Private Functions***********************************/

/********************************Public Functions***********************************/

// G8RTOS_InitSemaphore
// Initializes semaphore to a value. This is a critical section!
// Param "s": Pointer to semaphore
// Param "value": Value to initialize semaphore to
// Return: void
void G8RTOS_InitSemaphore(semaphore_t* s, int32_t value) {
   // your stuff goes here
    *s = value;    
}

// G8RTOS_WaitSemaphore
// Waits on the semaphore to become available, decrements value by 1. This is a
// critical section!
// Currently spin-locks.
// Param "s": Pointer to semaphore
// Return: void
void G8RTOS_WaitSemaphore(semaphore_t* s) {
    // your stuff goes here
    int32_t status = StartCriticalSection();
    if (*s <= 0) {
        G8RTOS_Block(s);
        EndCriticalSection(status);
        G8RTOS_Yield();
        return;
    } 
    (*s)--;
    EndCriticalSection(status);
}

// G8RTOS_SignalSemaphore
// Signals that the semaphore has been released by incrementing the value by 1.
// This is a critical section!
// Param "s": Pointer to semaphore
// Return: void
void G8RTOS_SignalSemaphore(semaphore_t* s) {
    // your stuff goes here
    int32_t status = StartCriticalSection();
    bool unblock_successful = G8RTOS_Unblock(s);
    if (unblock_successful) {
        EndCriticalSection(status);
        G8RTOS_Yield();
        return;
    } 
    (*s)++; 
    EndCriticalSection(status);
}


/********************************Public Functions***********************************/
