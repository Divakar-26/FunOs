#include "screen.h"
#include "../cpu/ports.h"
#include"../libc/string.h"
#include"../libc/mem.h"

//private functions
int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;//High byte: << 8
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; //Position * size of character cell
}
void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    char *videoMem = (char *) VIDEO_ADDRESS;

    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        videoMem[i * 2] = ' ';       // Character space
        videoMem[i * 2 + 1] = 0x07;  // White-on-black
    }

    // Reset cursor to (0,0)
    set_cursor_offset(0);
}

int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        col = 0;
        offset = get_offset(col, row + 1);
    } else {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }


    //scrolling
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            memory_copy((char *)get_offset(0, i) + VIDEO_ADDRESS,
                        (char *)get_offset(0, i-1) + VIDEO_ADDRESS,
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = (char *)get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }


    set_cursor_offset(offset);  // Update the cursor position
    return offset;
}


void kprint_at(char * message, int col, int row){
    int offset;
    if(col >= 0 && row >= 0){
        offset = get_offset(col, row);
    }
    else{
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while(message[i] != 0){
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char * message){
    kprint_at(message, -1, -1);
}

void kprint_color(char * message, char * color){

    int trueColor = get_color_from_string(color);

    int col = -1;
    int row = -1;
    int offset;
    if(col >= 0 && row >= 0){
        offset = get_offset(col, row);
    }
    else{
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while(message[i] != 0){
        offset = print_char(message[i++], col, row, trueColor);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }


void kprint_backspace() {
    int offset = get_cursor_offset()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(' ', col, row, WHITE_ON_BLACK);
    set_cursor_offset(offset);
}

int get_color_from_string(char * color){

    toLower(color);


    if(strcmp(color, "green") == 0) return GREEN_ON_BLACK;
    if(strcmp(color, "red") == 0 ) return RED_ON_BLACK;
    if(strcmp(color, "blue") == 0 ) return BLUE_ON_BLACK;
    if(strcmp(color, "cyan") == 0 ) return CYAN_ON_BLACK;
    if(strcmp(color, "yellow") == 0 ) return YELLOW_ON_BLACK;
    if(strcmp(color, "brown") == 0 ) return BROWN_ON_BLACK;
    if(strcmp(color, "magenta") == 0 ) return MAGENTA_ON_BLACK;
    if(strcmp(color, "lightblue") == 0 ) return LIGHTBLUE_ON_BLACK;
    if(strcmp(color, "lightgreen") == 0 ) return LIGHTGREEN_ON_BLACK;

    return WHITE_ON_BLACK;
}