// void dummy_test_entrypoint(){

// }

// void print_string(const char message[]){
//     volatile char* vidmem = (volatile char*) 0xB8000;
//     for (int i = 0; message[i] != '\0'; i++) {
//         vidmem[i * 2] = message[i];  // Write A, B, C, D, E
//         vidmem[i * 2 + 1] = 0x07;  // Attribute (white)
//     }
// }

// void main() {
//     const char message[] = "HELLO";
//     print_string(message);
// }

#include "../drivers/screen.h"
#include"util.h"
#include"../cpu/isr.h"
#include"../cpu/idt.h"
#include"../cpu/timer.h"
#include"../drivers/keyboard.h"
// #define VIDEO_ADDRESS 0xA0000
// void draw_pixel(int x, int y, unsigned char color) {
//     unsigned char *video_memory = (unsigned char*)VIDEO_ADDRESS;
    
//     // Calculate the pixel offset
//     int offset = (y * 320) + x;  // 320 is the screen width in pixels
    
//     // Set the pixel color at the computed offset
//     video_memory[offset] = color;
// }


void main() {
    clear_screen();
    isr_install();
    /* Test the interrupts */
    __asm__ volatile("sti");
    // init_timer(1);
    init_keyboard();


}