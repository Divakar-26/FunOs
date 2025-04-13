#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include "../libc/font.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
static char key_buffer[256];

static bool prompt_printed = false;
bool shift_pressed = false;
bool ctrl_pressed = false;

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

const char sc_ascii_shifted[] = {
    '?', '?', '!', '@', '#', '$', '%', '^', '&', '*',
    '(', ')', '_', '+', '?', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '?',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?', '?', '*', '?', ' ' 
};

void print_letter(u8 scancode)
{
    if (scancode <= SC_MAX)
    {
        kprint(sc_name[scancode]);
    }
    else if (scancode <= SC_MAX + 0x80)
    {
        kprint("key up ");
        print_letter(scancode - 0x80); // Recursively call for readable key
    }
    else
    {
        kprint("Unknown key");
    }
}

static void keyboard_callback(registers_t regs)
{
    u8 scancode = port_byte_in(0x60);

    // Handle key release
    if (scancode & 0x80)
    {
        u8 key = scancode & 0x7F; // Remove release bit

        if (key == 0x2A || key == 0x36)
        { // LShift or RShift
            shift_pressed = false;
        }
        else if (key == 0x1D)
        { // LCtrl
            ctrl_pressed = false;
        }

        // Don't print anything on key release
        return;
    }

    // Handle key press
    if (scancode == 0x2A || scancode == 0x36)
    { // LShift or RShift
        shift_pressed = true;
        return;
    }

    if (scancode == 0x1D)
    { // LCtrl
        ctrl_pressed = true;
        return;
    }

    if(scancode == 0x48){
        kprint("UP ");
        return;
    }
    else if(scancode == 0x50){
        kprint("DOWN ");
        return;
    }
    else if(scancode == 0x4B){
        kprint("LEFT ");
        return;
    }
    else if(scancode == 0x4D){
        kprint("RIGHT ");
        return;
    }

    if(scancode == 0x0F){
        kprint("    ");
        return;
    }

    // Handle normal key press
    if (scancode == BACKSPACE)
    {
        if(key_buffer[0] == '\0'){
            return;
        }
        kprint_backspace();
        backspace(key_buffer);
        
    }
    else if (scancode == ENTER)
    {
        kprint("\n");
        user_input(key_buffer);
        key_buffer[0] = '\0';
    }
    else
    {
        char letter = shift_pressed ? sc_ascii_shifted[scancode] : sc_ascii[scancode];

        if (ctrl_pressed)
        {
            if (letter == 'C' || letter == 'c')
            {
                kprint("Ctrl + C pressed (copy)\n");
                // Implement your copy logic
                return;
            }
            if (letter == 'V' || letter == 'v')
            {
                kprint("Ctrl + V pressed (paste)\n");
                // Implement your paste logic
                return;
            }
            if(letter == 'L' || letter == 'l'){

                clear_screenGraphics();


                return;
            }
        }

        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}