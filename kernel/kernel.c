#include <stdint.h>
#include "../drivers/screen.h"
#include"../libc/util.h"
#include"../cpu/isr.h"
#include"../cpu/idt.h"
#include"../cpu/timer.h"
#include"../drivers/keyboard.h"
#include"../cpu/ports.h"
#include"../libc/string.h"
#include"kernel.h"
#include "../libc/font.h"
// #define VIDEO_ADDRESS 0xA0000
// void draw_pixel(int x, int y, unsigned char color) {



void main() {

    isr_install();
    irq_install();
    kprint("Type something\n"
        "Type SHUTDOWN to halt the CPU");
    clear_screen();
    kprint_color("\n> ", 0x0A);
}

void user_input(char *input) {
    if (strcmp(input, "SHUTDOWN") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(strcmp(input, "CLEAR") == 0){
        clear_screen();
    }
    // kprint("You said: ");
    // kprint(input);
    kprint_color(">", 0x0A);
}
