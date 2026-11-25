/*

 _____ _____ _   _  ___   _   _  _____ _____ _____ _____ _   _ 
|_   _|_   _| | | |/ _ \ | \ | |/  ___|_   _|  ___|_   _| \ | |
  | |   | | | | | / /_\ \|  \| |\ `--.  | | | |__   | | |  \| |
  | |   | | | | | |  _  || . ` | `--. \ | | |  __|  | | | . ` |
  | |  _| |_\ \_/ / | | || |\  |/\__/ / | | | |___ _| |_| |\  |
  \_/  \___/ \___/\_| |_/\_| \_/\____/  \_/ \____/ \___/\_| \_/
                                                               
* ts_player.c
*  
* File that handles player control
* Created by: Landen Parke
* Date: 11/24/2026
*/

#include "../inc/ts_player.h"
#include "../inc/ts.h"

#include <stdint.h>

void player_getJoystick(int8_t *data)
{
    uint32_t joystick_xy[2];
    JOYSTICK_GetXY(joystick_xy);
        int16_t joystickX = (int16_t)((joystick_xy[0] & 0x0000FFFF) - 2000) / 1000;
        int16_t joystickY = -(int16_t)((joystick_xy[1] & 0x0000FFFF) - 2000) / 1000;
        if (joystickX > 1) joystickX = 1;
        if (joystickX < -1) joystickX = -1;
        if (joystickY > 1) joystickY = 1;
        if (joystickY < -1) joystickY = -1;
        data[0] = joystickX;
        data[1] = joystickY;
}