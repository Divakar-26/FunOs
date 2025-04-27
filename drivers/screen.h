#ifndef SCREEN_H
#define SCREEN_H

#include"../libc/colors.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// SCREEN I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

//function which should be private
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

void clear_screen();
void kprint_at(char * message, int col, int row);
void kprint(char * message);
void kprint_backspace();
void kprint_color(char * message, char * color);
int get_color_from_string(char * color);

#endif