#include"idt.h"
#include"../libc/util.h"

idt_gate_t idt[IDT_ENTRIES];  // This is the global IDT table
idt_register_t idt_reg;       // This is the global IDT register

void set_idt_gate(int n, u32 handler){
    idt[n].low_offset = low_16(handler);        //Store lowr 16 bits of the handler address
    idt[n].sel = KERNEL_CS;                     // Use kernel code segment (0x08)
    idt[n].always0 = 0;                         // must always be 0
    idt[n].flags = 0x8E;                        // Interrupt gate (32-bit, present)
    idt[n].high_offset = high_16(handler);      // store higher 16 bits of handler address
}

void set_idt(){
    idt_reg.base = (u32) &idt;                  // Store the base address of the IDT
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;           // Set IDT size 

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}