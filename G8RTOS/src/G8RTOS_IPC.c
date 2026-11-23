/*
 * G8RTOS_IPC.c
 *
 *  Created on: Oct 8, 2026
 *      Author: Landen
 */

/************************************Includes***************************************/
#include "./../G8RTOS_IPC.h"
#include "./../../G8RTOS/G8RTOS_Semaphores.h"
/************************************Includes***************************************/

/******************************Data Type Definitions********************************/
/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
G8RTOS_FIFO_t FIFO_array[MAX_NUM_FIFOS];
G8RTOS_FIFO_t *JOYSTICK_FIFO = &FIFO_array[0];
G8RTOS_FIFO_t *SPAWNCOOR_FIFO = &FIFO_array[1];

/****************************Data Structure Definitions*****************************/

/***********************************Externs*****************************************/
/***********************************Externs*****************************************/

/********************************Public Variables***********************************/
/********************************Public Variables***********************************/

/*******************************Private Variables***********************************/
/*******************************Private Variables***********************************/

/********************************Public Functions***********************************/
// G8RTOS_InitFIFO
// Initializes FIFO
// Returns: void
void G8RTOS_InitFIFO(G8RTOS_FIFO_t *fifo_sel) {
    fifo_sel->head = &fifo_sel->buffer[0];
    fifo_sel->tail = &fifo_sel->buffer[0];
    fifo_sel->lostdata_count = 0;
    G8RTOS_InitSemaphore(&(fifo_sel->size), 0);
    G8RTOS_InitSemaphore(&(fifo_sel->mutex), 1);
}
// G8RTOS_ReadFIFO
// Consumes one value from FIFO
// Return: some 32 bit integer
uint32_t G8RTOS_ReadFIFO(G8RTOS_FIFO_t *fifo_sel) {
    G8RTOS_WaitSemaphore(&fifo_sel->size);
    G8RTOS_WaitSemaphore(&fifo_sel->mutex);
    uint32_t data = *fifo_sel->head;
    if (fifo_sel->head != &fifo_sel->buffer[FIFO_MAX_SIZE - 1]) {
        fifo_sel->head++;
    } else {
        fifo_sel->head = &fifo_sel->buffer[0];
    }
    G8RTOS_SignalSemaphore(&fifo_sel->mutex);
    return data;
}
// G8RTOS_WriteFIFO
// Write some value to FIFO
// return: void
void G8RTOS_WriteFIFO(G8RTOS_FIFO_t *fifo_sel, uint32_t data) {
    G8RTOS_WaitSemaphore(&fifo_sel->mutex);
    if (fifo_sel->size > FIFO_MAX_SIZE - 1) {
            fifo_sel->lostdata_count++;
    } else G8RTOS_SignalSemaphore(&fifo_sel->size); // if not full inc size
    *fifo_sel->tail = data;
    if (fifo_sel->tail != &fifo_sel->buffer[FIFO_MAX_SIZE - 1]) {
        fifo_sel->tail++;
    } else {
        fifo_sel->tail = &fifo_sel->buffer[0];
    }
    G8RTOS_SignalSemaphore(&fifo_sel->mutex);

}

void G8RTOS_WriteFIFO_Mutexless(G8RTOS_FIFO_t *fifo_sel, uint32_t data) {
    if (fifo_sel->size > FIFO_MAX_SIZE - 1) {
            fifo_sel->lostdata_count++;
    } else {
        if (!G8RTOS_Unblock(&fifo_sel->size)) {
            fifo_sel->size++; // if not full inc size
        }
    }
    *fifo_sel->tail = data;
    if (fifo_sel->tail != &fifo_sel->buffer[FIFO_MAX_SIZE - 1]) {
        fifo_sel->tail++;
    } else {
        fifo_sel->tail = &fifo_sel->buffer[0];
    }
}
/********************************Public Functions***********************************/
