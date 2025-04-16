#include"graphics.h"
#include"string.h"



typedef enum
{
    BLACK = 0x00,
    BLUE = 0x01,
    GREEN = 0x02,
    CYAN = 0x03,
    RED = 0x04,
    MAGENTA = 0x05,
    BROWN = 0x06,
    LIGHT_GRAY = 0x07,
    DARK_GRAY = 0x08,
    LIGHT_BLUE = 0x09,
    LIGHT_GREEN = 0x0A,
    LIGHT_CYAN = 0x0B,
    LIGHT_RED = 0x0C,
    LIGHT_MAGENTA = 0x0D,
    YELLOW = 0x0E,
    WHITE = 0x0F
} VGA_Color;

void GdrawPixel(int x, int y, int color)
{
    VGA[y * SCREEN_WIDTH + x] = color;
}

void G_clear_screen(char * color){

    uint8_t c = getColorFromString(color);

    for(int i = 0; i < SCREEN_HEIGHT; i++){
        for(int j = 0; j < SCREEN_WIDTH; j++){
            GdrawPixel(j, i, c);
        }
    }
}

uint8_t getColorFromString(char * color){
    if(strcmp(color, "BLACK") == 0 || strcmp(color, "black") == 0){
        return 0x00;
    }
    else if(strcmp(color, "RED") == 0 || strcmp(color, "red") == 0){
        return 0x04;
    }
    else if(strcmp(color, "MAGENTA") == 0 || strcmp(color, "magenta") == 0){
        return 0x05;
    }
    else if(strcmp(color, "BLUE") == 0 || strcmp(color, "blue") == 0){
        return 0x01;
    }
}

void G_draw_rect(struct rect r, char * color){

    uint8_t c = getColorFromString(color);

    for(int i = 0 ; i < r.h; i++){
        for(int j = 0; j < r.w; j++){
            GdrawPixel(r.x + j, r.y + i, c);
        }
    }
}

void G_draw_circle(struct circle c, char * color){

    uint8_t co = getColorFromString(color);
    for(int i= -c.r; i <= c.r; i++){
        for(int j = -c.r; j <= c.r; j++){
            if(i*i + j * j <= c.r*c.r){
                GdrawPixel(c.x + i, c.y + j, co);
            }
        }
    }

}