#ifndef TIMER_H
#define TIMER_H

#include "../libc/mem.h"
#include"types.h"
#include"../drivers/screen.h"
#include"../cpu/ports.h"

void init_timer(u32 freq);
u32 timer_get_ticks();
void timer_delay(u32 time_in_seconds);
#endif