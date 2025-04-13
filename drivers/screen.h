#include<stdint.h>
#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0x0f4

// SCREEN I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

//function which should be private
int print_char(char c, int col, int row, char attr);


void clear_screen();
void kprint_at(char * message, int col, int row);
void kprint(const char * message);
void kprint_backspace();
void kprint_color(char * s, uint8_t color);

#endif