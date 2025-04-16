#include "ports.h"
#include"beep.h"

void beep() {
    u32 frequency = 1000;
    u32 divisor = 1193180 / frequency;

    // Command byte for PIT channel 2
    port_byte_out(0x43, 0xB6);

    // Set frequency divisor for channel 2
    port_byte_out(0x42, (u8)(divisor & 0xFF));        // Low byte
    port_byte_out(0x42, (u8)((divisor >> 8) & 0xFF)); // High byte

    // Enable the PC speaker (bits 0 and 1 of port 0x61)
    u8 tmp = port_byte_in(0x61);
    if ((tmp & 3) != 3) {
        port_byte_out(0x61, tmp | 3);
    }
}

void stop_beep() {
    u8 tmp = port_byte_in(0x61) & 0xFC;
    port_byte_out(0x61, tmp);
}