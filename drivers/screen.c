#include "screen.h"

#include "../cpu/ports.h"
#include"../libc/util.h"
#include"../libc/font.h"


void clear_screen() {

    clear_screenGraphics();
}

int print_char(char c, int col, int row, char attr) {

}

void kprint_at(char * message, int col, int row){

}

void kprint(char * message){
    printString(message);
}

void kprint_backspace(){
    printBackspace();
}

void kprint_color(char *s , uint8_t color){
    printStringColor(s, color);
}