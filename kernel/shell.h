// shell.h
#ifndef SHELL_H
#define SHELL_H

void shell_input(char *input); // Handles user input and calls corresponding commands
void shutdown_command(char *args); // Shutdown the system
void clear_command(char *args); // Clears the screen
void help_command(char *args); // Displays help message
void delay_command(char * args);
void show_tick_command();
void whoami_command();


#endif // SHELL_H
