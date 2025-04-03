#ifndef TIMER_H
#define TIMER_H

#include "../libc/util.h"
#include"types.h"
#include"../drivers/screen.h"
#include"../cpu/ports.h"

void init_timer(u32 freq);

#endif