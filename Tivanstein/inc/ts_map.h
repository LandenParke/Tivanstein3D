/*

 _____ _____ _   _  ___   _   _  _____ _____ _____ _____ _   _ 
|_   _|_   _| | | |/ _ \ | \ | |/  ___|_   _|  ___|_   _| \ | |
  | |   | | | | | / /_\ \|  \| |\ `--.  | | | |__   | | |  \| |
  | |   | | | | | |  _  || . ` | `--. \ | | |  __|  | | | . ` |
  | |  _| |_\ \_/ / | | || |\  |/\__/ / | | | |___ _| |_| |\  |
  \_/  \___/ \___/\_| |_/\_| \_/\____/  \_/ \____/ \___/\_| \_/
                                                               
* ts_map.h
*  
* File that handles processing of the 2D maps before they are raycasted to 3D
* Created by: Landen Parke
* Date: 11/23/2026
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct ray {
  uint16_t lineH;
  uint16_t color;
} ray;

int render_map();
int map_main(void);