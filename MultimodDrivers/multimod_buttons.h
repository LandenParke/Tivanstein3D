// multimod_buttons.h

#ifndef MULTIMOD_BUTTONS_H_
#define MULTIMOD_BUTTONS_H_

/************************************Includes***************************************/
#include <stdint.h>
/************************************Includes***************************************/

/*************************************Defines***************************************/
#define PCA9555_I2C_ADDR        0x23
#define PCA9555_SW1_bp          0x01
#define PCA9555_SW2_bp          0x02
#define PCA9555_SW3_bp          0x03
#define PCA9555_SW4_bp          0x04
/*************************************Defines***************************************/

/********************************Public Functions***********************************/
void MultimodButtons_Init(void);
uint8_t MultimodButtons_Get(void);
void MultimodButtons_IntEnable(void);
/********************************Public Functions***********************************/

#endif /* MULTIMOD_BUTTONS_H_ */