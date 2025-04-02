#ifndef IDT_H
#define IDT_H

#include "types.h"

// SEGEMENT SELECTOR
#define KERNEL_CS 0x08

typedef struct{
    u16 low_offset; //Lower 16 bit of handler function adress
    u16 sel; //kernel segment selector
    u8 always0; 
    /*
    The flags field is an 8-bit value that defines the type of interrupt and privilege level:
    Bit 7: Interrupt is present (1 = active, 0 = unused)
    Bits 6-5: Privilege level (0 = kernel, 3 = user)
    Bit 4: Set to 0 for interrupt gates
    Bits 3-0: Gate type
        0b1110 (14 in decimal) = 32-bit Interrupt Gate
    */
   u8 flags;
   u16 high_offset;
}__attribute__((packed)) idt_gate_t;

typedef struct{
    u16 limit; // size of the IDT (bytes - 1 )
    u32 base; // Address of the first entry in the IDT
}__attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_reg;


void set_idt_gate(int n, u32 handler);
void set_idt();

#endif