#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define KEYBOARD_DATA_PORT 0x60
#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];
static int shift = 0;
static int ctrl_pressed = 0;

char * prevCommand[256];
int i =0;
int totalCommand;

#define SC_MAX 57
const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};
const char sc_ascii_shifted[] = {'?', '?', '!', '@', '#', '$', '%', '^',
                                 '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                                 'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
                                 'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
                                 'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs)
{
    u8 scancode = port_byte_in(0x60);

    if (scancode & 0x80)
    {
        u8 real_scancode = scancode & 0x7F;
        if (real_scancode == 0x2A || real_scancode == 0x36)
        {
            // shift key up
            // kprint("SHIFT UP");
            shift = 0;
        }
        else if (real_scancode == 0x1D)
        { // LCtrl
            ctrl_pressed = 0;
            // kprint("CTRL RELEASED");
        }

        return;
    }
    else
    {
        if (scancode == 0x2A || scancode == 0x36)
        {
            // shift key DOWN
            // kprint("SHIFT");
            shift = 1;
            return;
        }
        else if (scancode == 0x1D)
        { // LCtrl
            // kprint("CTRL");
            ctrl_pressed = 1;
            return;
        }
        if (ctrl_pressed && scancode == BACKSPACE) {
            int size = strlen(key_buffer);
            if (size == 0) return;
            size--; 
            while (size > 0 && key_buffer[size] != ' ') {
                key_buffer[size] = '\0';
                size--;
                kprint_backspace(); 
            }
            key_buffer[size] = '\0'; 
            kprint_backspace(); 
            return;
        }
        if (scancode == 0x48) { 
            if (i > 0) {
                i--;
                while (strlen(key_buffer) > 0) {
                    backspace(key_buffer);
                    kprint_backspace();
                }
                strcpy(key_buffer, prevCommand[i]);
                kprint(key_buffer);
            }
            return;
        }
        if (scancode == 0x50) { 
            if (i < totalCommand - 1) {
                i++; 
                while (strlen(key_buffer) > 0) {
                    backspace(key_buffer);
                    kprint_backspace();
                }
                strcpy(key_buffer, prevCommand[i]);
                kprint(key_buffer);
            } else if (i == totalCommand - 1) {
                i++;
                while (strlen(key_buffer) > 0) {
                    backspace(key_buffer);
                    kprint_backspace();
                }
                memory_set(key_buffer, 0, sizeof(key_buffer));
            }
            return;
        }
        
        if (scancode == BACKSPACE)
        {
            if (strlen(key_buffer) > 0)
            {
                backspace(key_buffer);
                kprint_backspace();
            }
        }

        else if (scancode == ENTER)
        {
            kprint("\n");
            user_input(key_buffer);
            // key_buffer[0] = '\0';
            prevCommand[i] = strdup(key_buffer); // allocate and copy
            i++;
            totalCommand = i;
            memory_set(key_buffer, 0, sizeof(key_buffer));
        }
        else
        {
            char letter;
            if (shift)
            {
                letter = sc_ascii_shifted[(int)scancode];
            }
            else
            {
                letter = sc_ascii[(int)scancode];
            }
            /* Remember that kprint only accepts char[] */
            char str[2] = {letter, '\0'};
            append(key_buffer, letter);
            kprint(str);
        }
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}