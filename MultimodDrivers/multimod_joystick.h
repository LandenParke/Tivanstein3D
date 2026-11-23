// multimod_joystick.h

#ifndef MULTIMOD_JOYSTICK_H_
#define MULTIMOD_JOYSTICK_H_

/************************************Includes***************************************/
#include <stdint.h>
/************************************Includes***************************************/

/************************************Defines****************************************/
#define JOYSTICK_SWITCH_bm  0x04
/************************************Defines****************************************/

/********************************Public Functions***********************************/
void JOYSTICK_Init(void);
void JOYSTICK_IntEnable(void);
void JOYSTICK_GetXY(uint32_t *data);
/********************************Public Functions***********************************/

#endif /* MULTIMOD_JOYSTICK_H_ */
