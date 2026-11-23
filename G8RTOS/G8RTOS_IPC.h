/*
 * G8RTOS_IPC.h
 *
 *  Created on: Oct 8, 2026
 *      Author: Landen
 */

#ifndef G8RTOS_G8RTOS_IPC_H_
#define G8RTOS_G8RTOS_IPC_H_

/************************************Includes***************************************/
#include "./G8RTOS_Semaphores.h"
/************************************Includes***************************************/

/************************************Defines****************************************/
#define FIFO_MAX_SIZE       50
#define MAX_NUM_FIFOS       2
/************************************Defines****************************************/

/******************************Data Type Definitions********************************/
/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
typedef struct G8RTOS_FIFO_t {
    uint32_t buffer[FIFO_MAX_SIZE];
    uint32_t *head;
    uint32_t *tail;
    uint8_t lostdata_count;
    semaphore_t size;
    semaphore_t mutex;


} G8RTOS_FIFO_t;

/****************************Data Structure Definitions*****************************/

/***********************************Externs*****************************************/
/***********************************Externs*****************************************/

/********************************Public Variables***********************************/
/********************************Public Variables***********************************/

/*******************************Private Variables***********************************/
/*******************************Private Variables***********************************/

/********************************Public Functions***********************************/
void G8RTOS_InitFIFO(G8RTOS_FIFO_t *fifo_sel);
uint32_t G8RTOS_ReadFIFO(G8RTOS_FIFO_t *fifo_sel);
void G8RTOS_WriteFIFO(G8RTOS_FIFO_t *fifo_sel, uint32_t data);
void G8RTOS_WriteFIFO_Mutexless(G8RTOS_FIFO_t *fifo_sel, uint32_t data);

/********************************Public Functions***********************************/

#endif /* G8RTOS_G8RTOS_IPC_H_ */
