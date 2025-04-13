#include <stdint.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

#define VGA ((uint8_t*)0xA0000)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#define MAX_TEXT_COLS (SCREEN_WIDTH / CHAR_WIDTH)
#define MAX_TEXT_ROWS (SCREEN_HEIGHT / CHAR_HEIGHT)

void drawPixel(int x, int y, int color);
void drawRectangle(int x, int y, int w, int h, int color);
void draw_char(int x, int y, char c, uint8_t color);
void printString(char * s);
void clear_screen_graphics();
void incCursorY();
void incCursorX();
void scroll_screen();
void printBackspace();
void drawCursor();
void printStringColor(char * s, uint8_t color);
void clear_screenGraphics();

#endif