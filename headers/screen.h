#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

#define MAX_COLS 80
#define MAX_ROWS 25

#define COLOR_RED    0x0C
#define COLOR_RED_W    0xFC
#define COLOR_YELLOW 0x0E
#define COLOR_YELLOW_W 0xFE
#define COLOR_WHITE  0x0F
#define COLOR_WHITE_W  0xFF
#define COLOR_BLUE   0x09
#define COLOR_BLUE_W   0xF9
#define COLOR_GREEN  0x0A
#define COLOR_GREEN_W  0xFA
#define COLOR_CYAN_W   0xFB
#define COLOR_CYAN   0x0B
#define COLOR_GREY   0x07
#define COLOR_GREY_W   0xF7
#define COLOR_BLACK_W 0xF0
#define COLOR_MAGENTA 0x05
#define COLOR_BROWN   0x06
#define COLOR_DARK_GREY 0x08

extern char current_color;
extern int cursor_pos;

void print_string(const char* str, char color);
void print_char(char c, char color);
void newline();
void clear_screen();
void scroll();
void clear_screen2();
void print_int(uint32_t n, char color);

#endif