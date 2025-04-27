#include"timer.h"
#include "isr.h"
#include"../libc/function.h"
#include "../drivers/screen.h"

u32 tick = 0;
u32 last_second_tick = 0;
static void timer_callback(registers_t regs){
    tick++;
    // kprint("T");  // Tiny debug print to confirm it's firing
    UNUSED(regs);
}

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);


}

u32 timer_get_ticks(){
    return tick;
}

void timer_delay(u32 time_in_seconds) {
    u32 start_ticks = tick;  // Store the current tick count when delay starts
    u32 ticks_required = (time_in_seconds * 50) / 1000;  // Convert time to ticks (50 ticks per second)

    // Periodically check if enough ticks have passed
    asm volatile("sti");
    while ((tick - start_ticks) < ticks_required) {
        // Yield to the CPU, allowing other tasks to run (you can implement a 'yield' here)
        // Optionally, you can use `__asm__("hlt");` to put the CPU in a halt state temporarily
        // to save power, but ensure interrupts can still occur.
        // asm volatile("hlt");
        // asm volatile("sti");
    }
    // Once the required ticks have passed, return from the function

}

