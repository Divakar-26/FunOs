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


void main() {
    clear_screen();
    kprint_at("X", 1, 6);
    kprint_at("This text spans multiple lines", 75, 10);
    kprint_at("There is a line\nbreak", 0, 20);
    kprint("There is a line\nbreak");
    kprint_at("What happens when we run out of space?", 45, 24);
}