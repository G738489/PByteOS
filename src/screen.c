#include "screen.h"
char white_current_color = COLOR_WHITE;
char cyan_current_color = COLOR_CYAN;
char yellow_current_color = COLOR_YELLOW;
char green_current_color = COLOR_GREEN;
char red_current_color = COLOR_RED;
char* VIDEO_MEM = (char*)0xB8000;
char current_color = COLOR_GREY;
int cursor_pos = 0;

void print_char(char c, char color) {
    if (c == '\n') {
            int current_line = (cursor_pos / 2) / 80;
            cursor_pos = (current_line + 1) * 80 * 2;
            return; 
        }
    if (cursor_pos >= 4000) {
        scroll();
        cursor_pos = 3840;
    }
    VIDEO_MEM[cursor_pos++] = c;
    VIDEO_MEM[cursor_pos++] = color;
}

void print_string(const char* str, char color) {
    for (int i = 0; str[i] != '\0'; i++) print_char(str[i], color);
}

void newline() {
    cursor_pos = (cursor_pos / (MAX_COLS * 2) + 1) * (MAX_COLS * 2);
}
void print_int(uint32_t n, char color) {
    char str[11];
    int i = 9;
    str[10] = '\0';
    if (n == 0) {
        print_char('0', color);
        return;
    }
    while (n > 0) {
        str[i--] = (n % 10) + '0';
        n /= 10;
    }
    print_string(&str[i + 1], color);
}

void scroll() {
    int i = 0;
    while (i < 3840) {
        VIDEO_MEM[i] = VIDEO_MEM[i + 160];
        i++;
    }
    while (i < 4000) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = current_color;
        i += 2;
    }
}

void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = current_color;
    }
    cursor_pos = 0;
}
void clear_screen2() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = current_color;
    }
    cursor_pos = 0;
    print_string("Soli Deo Gloria",white_current_color);
    newline();
    print_string("Copyright (C) PByteOS (TM). All rights reserved [2026]",white_current_color); //0x0f
    newline();

}