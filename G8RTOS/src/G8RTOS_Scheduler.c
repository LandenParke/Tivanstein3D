// G8RTOS_Scheduler.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for scheduler functions

#include "../G8RTOS_Scheduler.h"

/************************************Includes***************************************/

#include <stdint.h>
#include <stdio.h>
#include "../G8RTOS_CriticalSection.h"
#include "../G8RTOS_Structures.h"
#include "../G8RTOS_IPC.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "../../threads.h"

/************************************Includes***************************************/

/********************************Private Variables**********************************/

// Thread Control Blocks - array to hold information for each thread
// initialize global variable here
tcb_t tcb_array[MAX_THREADS];
static ptcb_t tcb_array_periodic[MAX_THREADS_PERIODIC];

// Current Number of Threads currently in the scheduler
// initialize global variable here
static uint8_t num_threads = 0;
static uint8_t num_pthreads = 0;

extern G8RTOS_FIFO_t FIFO_array[MAX_NUM_FIFOS];
G8RTOS_FIFO_t *available_threads = &FIFO_array[0];

/********************************Private Variables**********************************/

/*******************************Private Functions***********************************/

// InitSysTick
// Init Systick to overflow every 1ms
// returns: void
static void InitSysTick(void) {
    // hint: use SysCtlClockGet() to get the clock speed without having to hardcode it!
   uint32_t clock_speed = SysCtlClockGet();
    // use timer interrupt to overflow every 1 ms (aka context switch ever 1 ms)
   uint32_t clock_period = clock_speed / 1000;
   SysTickPeriodSet(clock_period);
   SysTickEnable();
}

// G8RTOS_InterruptEnable
// Enables all interrupts for the G8RTOS
// Return: void
static void G8RTOS_InterruptEnable(void) {
    // Register Ints
    SysTickIntRegister(SysTick_Handler);
    IntRegister(FAULT_PENDSV, PendSV_Handler);
    IntPrioritySet(FAULT_PENDSV, 0xE0);
    IntPrioritySet(FAULT_SYSTICK, 0x00);
    // Enable ints
    IntEnable(FAULT_PENDSV);
    SysTickIntEnable();
    // IntMasterEnable();
}

// SetInitialStack
// Initializes stack values
// returns: void
static void G8RTOS_SetInitialStack(tcb_t* curr_thread) {
    uint32_t *stack = curr_thread->stack;
    curr_thread->sp = &stack[STACKSIZE - 16];
    stack[STACKSIZE -1] = THUMBBIT;
    stack[STACKSIZE - 3] = init_R14;
    stack[STACKSIZE - 4] = init_R12;
    stack[STACKSIZE - 5] = init_R3;
    stack[STACKSIZE - 6] = init_R2;
    stack[STACKSIZE - 7] = init_R1;
    stack[STACKSIZE - 8] = init_R0;
    stack[STACKSIZE - 9] = init_R11;
    stack[STACKSIZE - 10] = init_R10;
    stack[STACKSIZE - 11] = init_R9;
    stack[STACKSIZE - 12] = init_R8;
    stack[STACKSIZE - 13] = init_R7;
    stack[STACKSIZE - 14] = init_R6;
    stack[STACKSIZE - 15] = init_R5;
    stack[STACKSIZE - 16] = init_R4;
}
static void G8RTOS_SetThreadName(tcb_t *thread, char *name) {
    int i = 0;
    for (; i < MAX_NAME_LENGTH - 1; ++i) {
        if (name[i] != '\0') {
            thread->name[i] = name[i];
        } else break;
    }
    thread->name[i] = '\0';
}

// G8RTOS_DumbThreadInit
// Initializes the dumb thread
// returns: void
tcb_t dumb_thread;
static void G8RTOS_DumbThreadInit(void) {
    int32_t status = StartCriticalSection();
    G8RTOS_SetInitialStack(&dumb_thread);
    dumb_thread.id = 0x37;
    dumb_thread.priority = 255;
    G8RTOS_SetThreadName(&dumb_thread, "Dumb Thread");
    dumb_thread.stack[STACKSIZE - 2] = (int32_t)(Idle_Thread);
    EndCriticalSection(status);
}

static void G8RTOS_AvailableThreadsInit(void) {
    G8RTOS_InitFIFO(available_threads);
    for (int i = 0; i < MAX_THREADS; i++) {
        G8RTOS_WriteFIFO(available_threads, (uint32_t)(&tcb_array[i]));
    }
}
static void G8RTOS_AddAvailableThread(tcb_t *thread) {
    // G8RTOS_WriteFIFO(available_threads, (uint32_t)(thread)); // if this worked thatd be awesome but im a stupid chud
    
}

static tcb_t* G8RTOS_GetAvailableThread(void) {
    // tcb_t* empty_thread = (tcb_t*)(G8RTOS_ReadFIFO(available_threads));
    // return empty_thread;
    tcb_t* thread = NULL;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (!tcb_array[i].alive) {
            thread = &tcb_array[i];
            break;
        }
    }
    return thread;
}

static tcb_t* G8RTOS_GetReadyThread(void) {
    tcb_t* selected_thread = &dumb_thread;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (tcb_array[i].alive && !tcb_array[i].blocked && !tcb_array[i].asleep) {
            if (tcb_array[i].priority < selected_thread->priority || selected_thread == &dumb_thread) {
                selected_thread = &tcb_array[i];
            } else if (tcb_array[i].priority == selected_thread->priority && tcb_array[i].last_ran < selected_thread->last_ran) {
                selected_thread = &tcb_array[i];
            }
        }
    }
    return selected_thread;
}
static void G8RTOS_WakeUpThreads(void) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (tcb_array[i].asleep && tcb_array[i].wakeup_time <= SystemTime) {
            tcb_array[i].asleep = false;
        }
    }
}

static tcb_t* G8RTOS_IDSearch(threadID_t id) {
    tcb_t *thread = NULL;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (tcb_array[i].alive && tcb_array[i].id == id) {
            thread = &tcb_array[i];
        }
    }
    return thread;
}

static uint32_t G8RTOS_GetNumReady(void) {
    uint32_t num = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (tcb_array[i].alive && !tcb_array[i].blocked && !tcb_array[i].asleep) {
            num++;
        }
    }
    return num;
}

static void G8RTOS_ContextSwitch(void) {
    HWREG(NVIC_INT_CTRL) |= NVIC_INT_CTRL_PEND_SV;
}

static ptcb_t* G8RTOS_FindAvailablePeriodicThread(void) {
    if (num_pthreads >=  MAX_THREADS_PERIODIC) return NULL;
    ptcb_t* ret_thread = &tcb_array_periodic[num_pthreads];
    num_pthreads++;
    return ret_thread;
}

// G8RTOS_InitTCB
// Initializes a tcb_t pointer value
// Returns: void
static void G8RTOS_InitTCB(tcb_t* curr_thread, threadID_t id, void (*threadToAdd)(void), uint8_t priority, char *string) {
    G8RTOS_SetInitialStack(curr_thread);
    curr_thread->stack[STACKSIZE - 2] = (int32_t)(threadToAdd);
    curr_thread->id = id;    
    curr_thread->alive = true;
    curr_thread->blocked = false;
    curr_thread->asleep = false;
    curr_thread->last_ran = SystemTime;
    curr_thread->wakeup_time = SystemTime;
    curr_thread->priority = priority;
    G8RTOS_SetThreadName(curr_thread, string);
}

// G8RTOS_RunPeriodThreads
// Runs all periodic threads in order of lowest period first, then changes next execution_time
// Returns: void
static void G8RTOS_RunPeriodicThreads(void) {
    for (int i = 0; i < num_pthreads; i++) {
        if (tcb_array_periodic[i].execution_time <= SystemTime) {
            tcb_array_periodic[i].thread();
            tcb_array_periodic[i].execution_time += tcb_array_periodic[i].period;
        }
    }
}



/*******************************Private Functions***********************************/

/********************************Public Variables***********************************/

uint32_t SystemTime;

// reference currently running thread
tcb_t* CurrentlyRunningThread;

/********************************Public Variables***********************************/



/********************************Public Functions***********************************/



// G8RTOS_Init
// Initializes the RTOS by initializing system time.
// Return: void
void G8RTOS_Init() {
    // your code goes here
    SystemTime = 0;
    num_threads = 0;
    G8RTOS_DumbThreadInit();    // init dumby thread
    // G8RTOS_AvailableThreadsInit(); // Doesnt work idk why im a stupid chud

}

// G8RTOS_Launch
// Launches the RTOS.
// Return: error codes, 0 if none
sched_ErrCode_t G8RTOS_Launch() {
    // your code goes here
    // if (num_threads == 0) return NO_THREADS_SCHEDULED;
    uint32_t status = StartCriticalSection();
    CurrentlyRunningThread = G8RTOS_GetReadyThread();
    InitSysTick();
    G8RTOS_InterruptEnable();
    EndCriticalSection(status);

    // Call G8RTOS_Start()
    G8RTOS_Start();

    return NO_ERROR;
}

// G8RTOS_Scheduler
// Chooses next thread in the TCB. Round-robin scheduling.
// Return: void
void G8RTOS_Scheduler() {
    // stuff goes here (alot of stuff) w troll
    // IMPLEMENT ANTI DEADLOCK / SPINLOCK
    //      How can we prevent this?
    //      Solutions:
    //          Dumby thread < doing this cuz lazy
    //
    CurrentlyRunningThread = G8RTOS_GetReadyThread();

}

// G8RTOS_AddThread
// - Adds threads to G8RTOS Scheduler
// - Checks if there are still available threads to insert to scheduler
// - Initializes the thread control block for the provided thread
// - Initializes the stack for the provided thread to hold a "fake context"
// - Sets stack thread control block stack pointer to top of thread stack
// - Sets up the next and previous thread control block pointers in a round robin fashion
// Param void* "threadToAdd": pointer to thread function address
// Return: scheduler error code

// Lab 4 inputs: Thread ID (relative to tcb_array), function, priority,
sched_ErrCode_t G8RTOS_AddThread(threadID_t id, void (*threadToAdd)(void), uint8_t priority, char *string) {
    // your code goes here
    int32_t status = StartCriticalSection();
    // Thread initialization
    tcb_t* curr_thread = G8RTOS_GetAvailableThread(); 
    if (curr_thread == NULL) {
        EndCriticalSection(status);
        return THREAD_LIMIT_REACHED;
    }
    G8RTOS_InitTCB(curr_thread, id, threadToAdd, priority, string);
    
    num_threads++;

    EndCriticalSection(status);
    return NO_ERROR;
}

// G8RTOS_Add_AperiodicThread
// Adds a thread on interrupt, kills self on completion, handler lowk has everything
// Return: Scheduler error code
sched_ErrCode_t G8RTOS_Add_APeriodicEvent(void (*threadHandler)(void), uint32_t IRQn, uint8_t priority) {
    int32_t status = StartCriticalSection();
    if (IRQn < 6 || IRQn > 154) {
        EndCriticalSection(status);
        return IRQn_INVALID;
    }
    if (priority < 1 || priority > 6) {
        EndCriticalSection(status);
        return HWI_PRIORITY_INVALID;
    }
    IntRegister(IRQn, threadHandler);
    IntEnable(IRQn);
    IntPrioritySet(IRQn, priority << 5);
    EndCriticalSection(status);
    return NO_ERROR;
}
// G8RTOS_Add_periodicThread
// Adds a thread on some exec and period 
// Return: Scheduler error code
sched_ErrCode_t G8RTOS_Add_PeriodicEvent(uint32_t execution_time, uint32_t period, void (*threadToAdd)(void)) {
    int32_t status = StartCriticalSection();
    ptcb_t* thread = G8RTOS_FindAvailablePeriodicThread();
    if (thread == NULL) {
        EndCriticalSection(status);
        return THREAD_LIMIT_REACHED;
    }
    thread->execution_time = execution_time;
    thread->period = period;
    thread->thread = threadToAdd;
    EndCriticalSection(status);
    return NO_ERROR;
}

// SysTick_Handler
// Increments system time, sets PendSV flag to start scheduler.
// Return: void
void SysTick_Handler() {
    int32_t status = StartCriticalSection();
    SystemTime++;
    G8RTOS_RunPeriodicThreads();
    G8RTOS_WakeUpThreads();
    EndCriticalSection(status);
    G8RTOS_Yield(); // context switch
}

// G8RTOS_Sleep
// Sets a thread tcb to sleep until a specified number of system time is completed
// Return: void
void G8RTOS_Sleep(uint32_t sys_ticks) {
    uint32_t status = StartCriticalSection();
    CurrentlyRunningThread->wakeup_time = SystemTime + sys_ticks;
    CurrentlyRunningThread->asleep = true;
    EndCriticalSection(status);
}

// G8RTOS_Yield
// Yields the thread and initiates a context switch
// Return: Void
void G8RTOS_Yield(void) {
    CurrentlyRunningThread->last_ran = SystemTime;
    G8RTOS_ContextSwitch();
}

// G8RTOS_KillThread
// Function that kills a thread based on its ID
// Input: thread ID
// Return: ErrorCode
sched_ErrCode_t G8RTOS_KillThread(threadID_t id) {
    // if (G8RTOS_GetNumReady() == 1) return CANNOT_KILL_LAST_THREAD;
    uint32_t status = StartCriticalSection();
    tcb_t* thread = G8RTOS_IDSearch(id);
    if (thread == NULL) {
        EndCriticalSection(status);
        return THREAD_DOES_NOT_EXIST;
    }
    thread->alive = false;
    // G8RTOS_AddAvailableThread(thread);
    num_threads--;
    EndCriticalSection(status);
    return NO_ERROR;
}

// G8RTOS_KillSelf
// Function to kill currentlyrunningthread
// Input: void
// Return: ErrorCode
sched_ErrCode_t G8RTOS_KillSelf(void) {
    // if (G8RTOS_GetNumReady() == 1) return CANNOT_KILL_LAST_THREAD;
    uint32_t status = StartCriticalSection();
    CurrentlyRunningThread->alive = false;
    // G8RTOS_AddAvailableThread(CurrentlyRunningThread);
    num_threads--;
    G8RTOS_ContextSwitch();
    EndCriticalSection(status);
    return NO_ERROR;
}
