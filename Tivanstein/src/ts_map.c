/*

 _____ _____ _   _  ___   _   _  _____ _____ _____ _____ _   _ 
|_   _|_   _| | | |/ _ \ | \ | |/  ___|_   _|  ___|_   _| \ | |
  | |   | | | | | / /_\ \|  \| |\ `--.  | | | |__   | | |  \| |
  | |   | | | | | |  _  || . ` | `--. \ | | |  __|  | | | . ` |
  | |  _| |_\ \_/ / | | || |\  |/\__/ / | | | |___ _| |_| |\  |
  \_/  \___/ \___/\_| |_/\_| \_/\____/  \_/ \____/ \___/\_| \_/
                                                               
* ts_map.c
*  
* File that handles processing of the 2D maps before they are raycasted to 3D
* Created by: Landen Parke
* Date: 11/23/2026
*/

#include "../inc/ts_map.h"
#include "../inc/ts.h"


#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define MAP_SCALE               30
#define MAP_WIDTH               8
#define MAP_HEIGHT              8
#define NUM_RAYS                40
#define FIELD_OF_VIEW           60
#define DISPLAY_WIDTH           240
#define PI                      3.1415926535
#define PI_HALF                 PI/2
#define THREE_PI_HALF           3*PI/2
#define DR                      0.0174533
#define TIVANSTEIN_GREY         0x632C
#define TIVANSTEIN_RED          0x0019
#define TIVANSTEIN_BROWN        0x00E6
#define TIVANSTEIN_BLUE         0xD000 

static ray prev_ray[NUM_RAYS];

float px=60, py=60, pdx, pdy, pa = 0;
uint16_t pw=10, ph=10;

typedef enum Tivanstein_Colors{
    red = 1,
    blue = 2
} Tivanstein_Colors;

static const uint8_t debug_map[MAP_HEIGHT][MAP_WIDTH] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 2, 0, 0, 0, 0, 1,
    1, 0, 2, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

int render_map()
{
    for(int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (debug_map[i][j]) {
                DrawRectangle(j * MAP_SCALE + 1, i * MAP_SCALE + 1, MAP_SCALE - 2, MAP_SCALE -2, 0xFFFF);
            } else {
                DrawRectangle(j * MAP_SCALE + 1, i * MAP_SCALE + 1, MAP_SCALE - 2, MAP_SCALE -2, 0x0000);
            }
        }
    }
}

static uint16_t get_color(uint8_t value)
{
    switch (value) {
        case red:
            return TIVANSTEIN_RED;
        case blue:
            return TIVANSTEIN_BLUE;
        default:
            return ST7789_BLACK;
    }
}

static inline uint16_t decrease_color(uint16_t color, float div)
{
    uint16_t red = (color >> 0) & 0x001F;
    uint16_t green = (color >> 5) & 0x003F;
    uint16_t blue = (color >> 11) & 0x001F;
    red = (uint16_t)(red * div);
    green = (uint16_t)(green * div);
    blue = (uint16_t)(blue * div);
    return (red << 0) | (green << 5) | (blue << 11);
}


static inline int pos_to_ind(int x)
{
    return x / MAP_SCALE;
}

bool check_collision(int x, int y)
{
    int xl_i = pos_to_ind(x - pw/2);
    int yl_i = pos_to_ind(y - ph/2);
    int xh_i = pos_to_ind(x + pw/2 );
    int yh_i = pos_to_ind(y + ph/2);
    if (debug_map[yl_i][xl_i] || debug_map[yh_i][xh_i] || debug_map[yh_i][xl_i] || debug_map[yl_i][xh_i]) return false;
    return true;
}

static inline float dist(float ax, float ay, float bx, float by, float angle)
{
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) );
}

static inline void fill_span(int x, int w, int y0, int y1, uint16_t c)
{
    if (y1 > y0) DrawRectangle(x, y0, w, y1 - y0, c);
}

void drawRays3D()
{
    int r, mx, my, dof;
    float rx, ry, ra, xo, yo;
    ra = pa + DR*(FIELD_OF_VIEW/2);
    if (ra < 0) ra += 2*PI;
    if (ra > 2*PI) ra -= 2*PI;
    for(r=0; r < NUM_RAYS; r++)
    {
        /* Horizontal lines */
        dof = 0;
        bool hitH = false;
        uint8_t cellV=0, cellH=0;
        float disH = 1000000, hx=px, hy=py;
        float aTan = -1/tan(ra);
        if (ra > PI) { 
            ry = ((float)pos_to_ind(py) * MAP_SCALE - 0.0001);
            rx = (py-ry) * aTan + px;
            yo = -MAP_SCALE;
            xo = -yo *aTan;
        }
        if (ra < PI) {
            ry = ((float)pos_to_ind(py) * MAP_SCALE + MAP_SCALE);
            rx = (py-ry) * aTan + px;
            yo =  MAP_SCALE;
            xo = -yo *aTan;
        }
        if (fabs(ra) < 0.0001 || fabs(ra - PI) < 0.0001) { /* Horizontal */
            rx = px;
            ry = py;
            dof = 8;
        }
        while (dof < 8) 
        {
            if (rx < 0 || ry < 0 || rx >= MAP_WIDTH*MAP_SCALE || ry >= MAP_HEIGHT*MAP_SCALE) break;
            mx = pos_to_ind(rx);
            my = pos_to_ind(ry);
            if (debug_map[my][mx]) { /* hit wall */
                hitH = true;
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
                cellH = debug_map[my][mx];
                break;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
    
        /* Vertical lines */
        dof = 0;
        bool hitV = false;
        float disV = 1000000, vx=px, vy=py;
        float nTan = -tan(ra);
        if (ra > PI_HALF && ra < THREE_PI_HALF) { /* left */
            rx = ((float)pos_to_ind(px) * MAP_SCALE - 0.0001);
            ry = (px-rx) * nTan + py;
            xo = -MAP_SCALE;
            yo = -xo *nTan;
        }
        if (ra < PI_HALF || ra > THREE_PI_HALF) { /* right */
            rx = ((float)pos_to_ind(px) * MAP_SCALE + MAP_SCALE);
            ry = (px-rx) * nTan + py;
            xo =  MAP_SCALE;
            yo = -xo *nTan;
        }
        if (fabs(ra - PI_HALF) < 0.0001 || fabs(ra - THREE_PI_HALF) < 0.0001) { /* Vertical */
            rx = px;
            ry = py;
            dof = 8;
        }
        while (dof < 8) 
        {
            if (rx < 0 || ry < 0 || rx >= MAP_WIDTH*MAP_SCALE || ry >= MAP_HEIGHT*MAP_SCALE) break;
            mx = pos_to_ind(rx);
            my = pos_to_ind(ry);
            if (debug_map[my][mx]) { /* hit wall */
                hitV = true;
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
                cellV = debug_map[my][mx];
                break;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        float dis = 0;
        uint16_t wall_color = ST7789_BLACK;
        if (hitV && (!hitH || disV < disH)) { /* Vertical */
            rx=vx;
            ry=vy;
            dis = disV;
            wall_color = get_color(cellV);
        }
        else if (hitH) { /* Horizontal */
            rx=hx;
            ry=hy;
            dis = disH;
            wall_color = decrease_color(get_color(cellH), 0.75);
        }

        /* Draw 3D Walls */
        int lineH = 0;
        if (hitH || hitV) {
            dis = dis*cos(pa - ra); /* fisheye correction */
            float h = (dis < 1.0) ? 300.0 : (MAP_SCALE * 300 / dis);
            if (h > 300.0) h = 300.0;
            lineH = (int)h;
        }

        int x = r*(DISPLAY_WIDTH/NUM_RAYS);
        int w = DISPLAY_WIDTH/NUM_RAYS;
        int newBot = 150 - lineH/2;                 
        int newTop = newBot + lineH;                
        int oldBot = 150 - prev_ray[r].lineH/2;
        int oldTop = oldBot + prev_ray[r].lineH;

        /* New wall pixels */
        if (prev_ray[r].color != wall_color) {
            fill_span(x, w, newBot, newTop, wall_color);
        } else {
            if (newTop > oldTop) fill_span(x, w, oldTop, newTop, wall_color); 
            if (newBot < oldBot) fill_span(x, w, newBot, oldBot, wall_color); 
        }

        /* Old wall pixels no longer covered, draw black */
        if (oldTop > newTop) fill_span(x, w, (newTop > oldBot) ? newTop : oldBot, oldTop, TIVANSTEIN_GREY);
        if (oldBot < newBot) fill_span(x, w, oldBot, (newBot < oldTop) ? newBot : oldTop, TIVANSTEIN_BROWN); 

        prev_ray[r].lineH = lineH;
        prev_ray[r].color = wall_color;

        ra -= (FIELD_OF_VIEW/NUM_RAYS)*DR;
        if (ra < 0) ra += 2*PI;
        if (ra > 2*PI) ra -= 2*PI;
    }
}

static inline void draw_player(uint16_t color)
{
    DrawRectangle(px - pw/2, py - ph/2, pw, ph, color);
}

int map_main(void)
{
    int8_t joystick_xy[2];
    int16_t joystickX, joystickY;
    float new_x, new_y;
    float old_pa = 0, old_px = 0, old_py = 0;
    for (;;) 
    {
        old_pa = pa; old_px = px; old_py = py;
        player_getJoystick(joystick_xy);

        if (joystick_xy[0]*joystick_xy[0]) /* is abs 1 */
        {
            if (joystick_xy[0] == 1) pa -= PI/100;
            if (joystick_xy[0] == -1) pa += PI/100;
            if (pa < 0) pa = 2*PI - PI/100;
            if (pa > 2*PI) pa = 0;
        }
        if (joystick_xy[1]*joystick_xy[1]) 
        {
            pdx = ((float)joystick_xy[1]*cos(pa)*2);
            pdy = ((float)joystick_xy[1]*sin(pa)*2);
            new_x = px + pdx;
            new_y = py + pdy;
        }
        
        if (check_collision(new_x, new_y) && (new_x != px || new_y != py)) {
            px = new_x;
            py = new_y;
        }
        if (old_pa != pa || old_px != px || old_py != py) drawRays3D(ST7789_GREEN);
        SysCtlDelay(100000);
    }
    return 0;
}



void render_thread(void)
{

}