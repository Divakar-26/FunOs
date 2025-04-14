#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "kernel.h"
#include"../cpu/timer.h"


#define MAX_COMMANDS 10


typedef struct {
    const char *name;
    void (*function)(char *args);
} Command;


void shutdown_command(char *args) {
    kprint("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}

void clear_command(char *args) {
    clear_screen();
}
void help_command(char *args) {
    kprint("Available commands:\n");
    kprint("  SHUTDOWN - Stops the CPU\n");
    kprint("  CLEAR - Clears the screen\n");
    kprint("  HELP - Displays this message\n");
}

void delay_command(char * args){
    // timer_delay(500);

    int timeInSec = string_to_int(args);

    timer_delay((u32)timeInSec);

    u32 ticks = timer_get_ticks();
    char *time; u32_to_str(ticks, time);
    kprint(time);
    kprint("\n");
}

void show_tick_command(){
    u32 ticks = timer_get_ticks();
    char *time; u32_to_str(ticks, time);
    kprint(time);
    kprint("\n");
}

Command command_registry[MAX_COMMANDS] = {
    {"shutdown", shutdown_command},
    {"clear", clear_command},
    {"help", help_command},
    {"timer", delay_command},
    {"showTick", show_tick_command}
};

void shell_input(char * input){

    char input2[256];
    int i = 0;
    // Skip leading zeros
    while (input[i] == ' ') {
        i++;  // Skip over leading zeros
    }

    int j = 0; // Index for the input2 buffer
    while (input[i] != '\0' && j < 255) {
        input2[j] = input[i];
        i++;
        j++;
    }

    input2[j] = '\0';

    // Now check the command
    char *args = my_strchr(input2, ' ');
    if (args != NULL) {
        *args = '\0';  // Null terminate command
        args++;        // Move pointer past the space
    }

    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (strcmp(input2, command_registry[i].name) == 0) {
            command_registry[i].function(args);
            kprint_color(">", 0x0A); // Prompt again after executing command
            return;
        }
    }

    // kprint("You said: ");
    // kprint(input);


    kprint("Unknown command: ");
    kprint(input2);
    kprint("\n");
    kprint_color(">", 0x0A); // Prompt again
}