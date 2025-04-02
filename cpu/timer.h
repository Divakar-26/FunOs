#ifndef TIMER_H
#define TIMER_H

#include "../kernel/util.h"
#include"types.h"
#include"../drivers/screen.h"
#include"../drivers/ports.h"

void init_timer(u32 freq);

#endif