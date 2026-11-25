/*

 _____ _____ _   _  ___   _   _  _____ _____ _____ _____ _   _ 
|_   _|_   _| | | |/ _ \ | \ | |/  ___|_   _|  ___|_   _| \ | |
  | |   | | | | | / /_\ \|  \| |\ `--.  | | | |__   | | |  \| |
  | |   | | | | | |  _  || . ` | `--. \ | | |  __|  | | | . ` |
  | |  _| |_\ \_/ / | | || |\  |/\__/ / | | | |___ _| |_| |\  |
  \_/  \___/ \___/\_| |_/\_| \_/\____/  \_/ \____/ \___/\_| \_/
                                                               
* ts_G8RTOS.h
*  
* File that sets up G8RTOS implementation
* Created by: Landen Parke
* Date: 11/23/2026
*/

#pragma once

#include "../../G8RTOS/G8RTOS.h"
#include "../../MultimodDrivers/multimod.h"

#define DISPLAY_WIDTH_MAX           320
#define DISPLAY_HEIGHT_MAX          240

static inline int AddThread(threadID_t id, void (*threadToAdd)(void), uint8_t priority, char* string) {
    return G8RTOS_AddThread(id, threadToAdd, priority, string);
}

static inline sched_ErrCode_t AddAperiodicThread(void (*threadHandler)(void), uint32_t IRQn, uint8_t priority) {
    return G8RTOS_Add_APeriodicEvent(threadHandler, IRQn, priority);
}

static inline sched_ErrCode_t AddPeriodicThread(uint32_t execution_time, uint32_t period, void(*threadToAdd)(void)) {
    return G8RTOS_Add_PeriodicEvent(execution_time, period, threadToAdd);
}

static inline void Sleep(uint32_t system_ticks) {
    G8RTOS_Sleep(system_ticks);
}

static inline void Wait(semaphore_t* s) {
    G8RTOS_WaitSemaphore(s);
}

static inline void Signal(semaphore_t* s) {
    G8RTOS_SignalSemaphore(s);
}

static inline void Yield(void) {
    G8RTOS_Yield();
}

static inline sched_ErrCode_t Kill(threadID_t id) {
    return G8RTOS_KillThread(id);
}

static inline sched_ErrCode_t KillSelf(void) {
    return G8RTOS_KillSelf();
}

static inline DrawPixel(uint16_t x, uint16_t y, uint16_t color) {    
    ST7789_DrawPixel(x, y, color);
}

static inline DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ST7789_DrawRectangle(x, y + 20, w, h, color);
}

static inline DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    ST7789_Line(x0, y0, x1, y1, color);
}

static inline DrawAngledLine(uint16_t x, uint16_t y, float angle, uint16_t length, uint16_t color) {
    ST7789_DrawAngledLine(x, y + 20, angle, length, color);
}