#include<stdint.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

#define VGA ((uint8_t*)0xA0000)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

struct rect{
    int x,y;
    int w,h;
};

struct circle{
    int x, y;
    int r;
};

void GdrawPixel(int x, int y, int color);
void G_clear_screen(char * color);
uint8_t getColorFromString(char * color);
void G_draw_rect(struct rect r, char * color);
void G_draw_circle(struct circle c, char * color);
void G_render();

#endif