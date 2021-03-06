#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#ifdef PC
#define TILE_SIZE 64
#define WIDTH 640
#define HEIGHT 480
#define FONT_H 16
#define FONT_W 16
#endif

#ifdef DC
#define TILE_SIZE 32
#define WIDTH 320
#define HEIGHT 240
#define FONT_H 8
#define FONT_W 8
#define DC_BUTTON_A 0
#define DC_BUTTON_B 1
#define DC_BUTTON_X 2
#define DC_BUTTON_Y 3
#define DC_BUTTON_START 4

#define DC_STICK_Y_AXIS 0
#define DC_STICK_X_AXIS 1
#define DC_SHOULDER_L 3
#define DC_SHOULDER_R 2

#define DC_HAT_CROSS 0
#define DC_HAT_CROSS_UP 1
#define DC_HAT_CROSS_DOWN 4
#define DC_HAT_CROSS_LEFT 8
#define DC_HAT_CROSS_RIGHT 2
#endif

#endif // CONSTANTS_H_INCLUDED
