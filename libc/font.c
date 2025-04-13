#include "font.h"
#include "font8x8.h"

typedef enum
{
    COLOR_BLACK = 0x00,
    COLOR_BLUE = 0x01,
    COLOR_GREEN = 0x02,
    COLOR_CYAN = 0x03,
    COLOR_RED = 0x04,
    COLOR_MAGENTA = 0x05,
    COLOR_BROWN = 0x06,
    COLOR_LIGHT_GRAY = 0x07,
    COLOR_DARK_GRAY = 0x08,
    COLOR_LIGHT_BLUE = 0x09,
    COLOR_LIGHT_GREEN = 0x0A,
    COLOR_LIGHT_CYAN = 0x0B,
    COLOR_LIGHT_RED = 0x0C,
    COLOR_LIGHT_MAGENTA = 0x0D,
    COLOR_YELLOW = 0x0E,
    COLOR_WHITE = 0x0F
} VGA_Color;

int cursorX = 0;
int cursorY = 0;
int prevCursorX = 0;
int prevCursorY = 0;

void drawPixel(int x, int y, int color)
{
    VGA[y * SCREEN_WIDTH + x] = color;
}

void drawRectangle(int x, int y, int w, int h, int color)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            VGA[(y + i) * SCREEN_WIDTH + (x + j)] = color;
        }
    }
}

void draw_char(int x, int y, char c, uint8_t color)
{
    const uint8_t *glyph = font8x8_basic[(int)c];
    for (int row = 0; row < 8; row++)
    {
        uint8_t row_data = glyph[row];
        for (int col = 0; col < 8; col++)
        {
            if (row_data & (1 << col))
            {
                drawPixel(x + col, y + row, color);
            }
        }
    }
}

void printString(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '\n')
        {
            cursorX = 0;
            cursorY += CHAR_HEIGHT;

            // Scroll if we go beyond screen height
            if (cursorY + CHAR_HEIGHT > SCREEN_HEIGHT)
            {
                scroll_screen();
            }

            continue; // skip drawing this character
        }

        draw_char(cursorX, cursorY, s[i], COLOR_WHITE);
        cursorX += CHAR_WIDTH;

        if (cursorX + CHAR_WIDTH > SCREEN_WIDTH)
        {
            cursorX = 0;
            cursorY += CHAR_HEIGHT;

            if (cursorY + CHAR_HEIGHT > SCREEN_HEIGHT)
            {
                scroll_screen();
            }
        }
    }
    drawCursor();
}

void printStringColor(char *s, uint8_t color)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '\n')
        {
            cursorX = 0;
            cursorY += CHAR_HEIGHT;

            // Scroll if we go beyond screen height
            if (cursorY + CHAR_HEIGHT > SCREEN_HEIGHT)
            {
                scroll_screen();
            }

            continue; // skip drawing this character
        }

        draw_char(cursorX, cursorY, s[i], color);
        cursorX += CHAR_WIDTH;

        if (cursorX + CHAR_WIDTH > SCREEN_WIDTH)
        {
            cursorX = 0;
            cursorY += CHAR_HEIGHT;

            if (cursorY + CHAR_HEIGHT > SCREEN_HEIGHT)
            {
                scroll_screen();
            }
        }
    }

    drawCursor();
}

void printString_at(int x, int y, char *s)
{
    cursorX = x;
    cursorY = y;
    printString(s);
}

void scroll_screen()
{
    // Move each row up by CHAR_HEIGHT pixels
    for (int y = 0; y < SCREEN_HEIGHT - CHAR_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            VGA[y * SCREEN_WIDTH + x] = VGA[(y + CHAR_HEIGHT) * SCREEN_WIDTH + x];
        }
    }

    // Clear the last CHAR_HEIGHT rows
    for (int y = SCREEN_HEIGHT - CHAR_HEIGHT; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            VGA[y * SCREEN_WIDTH + x] = COLOR_BLACK;
        }
    }

    cursorY -= CHAR_HEIGHT;
    if (cursorY < 0)
        cursorY = 0;
}

void incCursorY() { cursorY += CHAR_HEIGHT; }
void incCursorX() { cursorX += CHAR_WIDTH; }

void printBackspace()
{
    if (cursorX == 0)
    {
        if (cursorY == 0)
            return; // Already at top-left, do nothing
        cursorY -= CHAR_HEIGHT;
        cursorX = SCREEN_WIDTH - CHAR_WIDTH;
    }
    else
    {
        cursorX -= CHAR_WIDTH;
    }

    // Overwrite character with a black rectangle (erase)
    drawRectangle(cursorX, cursorY, CHAR_WIDTH, CHAR_HEIGHT, COLOR_BLACK);
    drawCursor();
}

void drawCursor()
{
    // Erase old cursor
    drawRectangle(prevCursorX, prevCursorY + CHAR_HEIGHT, CHAR_WIDTH, 2, COLOR_BLACK);

    // Draw new cursor (as underline at the bottom of character cell)
    drawRectangle(cursorX, cursorY + CHAR_HEIGHT, CHAR_WIDTH, 2, COLOR_WHITE);

    // Save current cursor position for next erase
    prevCursorX = cursorX;
    prevCursorY = cursorY;
}

void clear_screenGraphics()
{
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            drawPixel(j, i, COLOR_BLACK);
        }
    }

    cursorX = 0;
    cursorY = 0;

    printStringColor(">", 0x0A);
}
