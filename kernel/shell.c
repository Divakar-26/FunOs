#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "kernel.h"
#include "../cpu/timer.h"


#define MAX_COMMANDS 10

typedef struct
{
    const char *name;
    void (*function)(char *args);
} Command;

void shutdown_command(char *args)
{
    kprint("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}

void clear_command(char *args)
{
    clear_screen();   
}

void help_command(char *args)
{
    kprint("Available commands:\n");
    kprint("  SHUTDOWN - Stops the CPU\n");
    kprint("  CLEAR - Clears the screen\n");
    kprint("  HELP - Displays this message\n");
}

void delay_command(char *args)
{
    u32 phys_addr;
    u32 page = kmalloc(1000, 1, &phys_addr);
    char page_str[16] = "";
    hex_to_ascii(page, page_str);
    char phys_str[16] = "";
    hex_to_ascii(phys_addr, phys_str);
    kprint("Page: ");
    kprint(page_str);
    kprint(", physical address: ");
    kprint(phys_str);
    kprint("\n");
}

void show_tick_command()
{
    u32 tick = timer_get_ticks();   
    char * tickInString;
    u32_to_str(tick, tickInString);
    kprint(tickInString);
    kprint("\n");
}

void echo_color_command(char *args) {
    char color[20] = "white"; // Default to white
    char argument[200] = "";
    int i = 0;

    // Parse flags
    if (args[i] == '-') {
        i++; // skip '-'
        if (args[i] == 'c' && (args[i+1] == ' ' || args[i+1] == '\0')) {
            i += 2; // Skip 'c' and space
            int j = 0;
            // Get the color name
            while (args[i] != ' ' && args[i] != '\0') {
                color[j++] = args[i++];
            }
            color[j] = '\0';
        }
    }

    // Skip spaces before the actual message
    while (args[i] == ' ') i++;

    // Copy remaining args as the message
    int j = 0;
    while (args[i] != '\0') {
        argument[j++] = args[i++];
    }
    argument[j] = '\0';

    // Now print the message with the given color
    kprint_color(argument, color);
    kprint("\n");
}

Command command_registry[MAX_COMMANDS] = {
    {"shutdown", shutdown_command},
    {"clear", clear_command},
    {"help", help_command},
    {"timer", delay_command},
    {"showTick", show_tick_command},
    {"echo", echo_color_command}
};

void shell_input(char *input)
{

    char input2[256];
    int i = 0;
    // Skip leading zeros
    while (input[i] == ' ')
    {
        i++; // Skip over leading zeros
    }

    int j = 0; // Index for the input2 buffer
    while (input[i] != '\0' && j < 255)
    {
        input2[j] = input[i];
        i++;
        j++;
    }

    input2[j] = '\0';

    // Now check the command
    char *args = my_strchr(input2, ' ');
    if (args != NULL)
    {
        *args = '\0'; // Null terminate command
        args++;       // Move pointer past the space
    }

    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        if (strcmp(input2, command_registry[i].name) == 0)
        {
            command_registry[i].function(args);
            kprint_color(">", "GREEN"); // Prompt again after executing command
            return;
        }
    }

    // kprint("You said: ");
    // kprint(input);

    kprint(input2);
    kprint(": Command not found");
    kprint_color("\n>", "GREEN");
}