#include <stdint.h>
#include "../drivers/screen.h"
#include"../libc/mem.h"
#include"../cpu/isr.h"
#include"../cpu/idt.h"
#include"../cpu/timer.h"
#include"../drivers/keyboard.h"
#include"../cpu/ports.h"
#include"../libc/string.h"
#include"kernel.h"
#include"shell.h"
// #define VIDEO_ADDRESS 0xA0000
// void draw_pixel(int x, int y, unsigned char color) {



void main() {

    isr_install();
    irq_install();
    init_timer(50);
    asm volatile("sti");
    clear_screen();
    kprint("Type something\n"
        "Type SHUTDOWN to halt the CPU");
    kprint_color("\n>", "GREEN");

}

void user_input(char *input) {
    shell_input(input);
}
