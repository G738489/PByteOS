// PByteOS(TM) 0.1.2 Beta Preview
// Copyright (c) 2026 G738489.
// Licensed under the MIT License.
#include <stdint.h>
typedef unsigned int uint32_t;
char* VIDEO_MEM = (char*)0xB8000;
#define MAX_COLS 80
#define MAX_ROWS 25
unsigned int c = 0;                   
void print_string(const char* str, char color);
void print_char(char c, char color);
void newline();
int cursor_pos = 0;
char command_buffer[64];
int buffer_idx = 0;
int is_shift_pressed = 0;
int is_caps_lock = 0;
uint8_t read_clock_year(){
    uint8_t year;
    __asm__ volatile(
        "mov $0x09, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (year)
    );
    return year;

}

void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = 0x07;
    }
    cursor_pos = 0;
}

uint8_t read_clock_day(){
    uint8_t day;
    __asm__ volatile(
        "mov $0x07, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (day)
    );
    return day;

}

uint8_t read_clock_mes(){
    uint8_t mes;
    __asm__ volatile(
        "mov $0x08, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (mes)
    );
    return mes;

}

uint8_t read_clock_seculo(){
    uint8_t seculo;
    __asm__ volatile(
        "mov $0x32, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (seculo)
    );
    return seculo;

}

uint8_t read_clock_battery(){
    uint8_t power;
    __asm__ volatile(
        "mov $0x0D, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (power)
    );
    return power;

}
uint8_t read_clock_hour(){
    uint8_t hour;
    __asm__ volatile(
        "mov $0x04, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (hour)
    );
    return hour;
}

uint8_t read_clock_minutes(){
    uint8_t minutes;
    __asm__ volatile(
        "mov $0x02, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (minutes)
    );
    return minutes;
}

int starts_with(const char* buffer, const char* prefix) {
    while(*prefix) { 
        if(*prefix != *buffer) return 0; 
        prefix++;
        buffer++;
    }
    return 1;
}

int check_day(unsigned char day){ 
    if (day > 31){ 
        return 0;
    }
    if (day < 1){ 
        return 0;
    }
    return 1;
}

int check_hour(unsigned char hour){ 
    if (hour > 23){ 
        return 0;
    }
    return 1;
}

int check_minutes(unsigned char minutes){ 
    if (minutes > 59){ 
        return 0;
    }
    return 1;
}
int check_mes(unsigned char mes){ 
    if (mes > 12){ 
        return 0;
    }
    if (mes < 1){ 
        return 0;
    }
    return 1;
}

int check_seculo(unsigned char seculo){ 
    if (seculo > 99){ 
        return 0;
    }
    return 1;
}
int check_ano(unsigned char ano){ 
    if (ano > 99){ 
        return 0;
    }
    return 1;
}
void crash_screen(char* reason){
    clear_screen();
    int i = 0; 
    while(i < 80 * 25){
        VIDEO_MEM[i * 2] = ' ';
        VIDEO_MEM[i * 2 + 1] = 0x1F;
        i++;
    }
    if(i >= 80*25){
        print_string("PByteOS found a critical error and needed to shutdown.",0x1F);
        newline();
        print_string("Restart your pc.",0x1F);
        newline();
        print_string("Reason: ",0x1F);
        print_string(reason,0x1F);
        __asm__ volatile("cli; hlt");
     }
    }
unsigned char keyboard_map[128] = {  // mapped characters

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',

};

unsigned char keyboard_map_shift[128] = {  // mapped characters

    0,  27, '!', '@', '#', '$', '%', '6', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '/', 0, '*', 0, ' ',

};

unsigned char keyboard_map_capslock[128] = {  // mapped characters

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ',

};
int str_compare(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s1[i] == s2[i]) i++;
    return (s1[i] == s2[i]);
}

void print_char(char c, char color) {
    VIDEO_MEM[cursor_pos++] = c;
    VIDEO_MEM[cursor_pos++] = color;
}

void print_string(const char* str, char color) {
    for (int i = 0; str[i] != '\0'; i++) print_char(str[i], color);
}

void clear_screen2() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = 0x07;
    }
    cursor_pos = 0;
    print_string("Copyright (C) PByteOS (TM). All rights reserved [2026]", 0x0F);
    newline();

}
void newline() {
    cursor_pos = (cursor_pos / (MAX_COLS * 2) + 1) * (MAX_COLS * 2);
}
void time_cmd(){
        uint8_t hour1;
        uint8_t minutes1;
        uint8_t minutes2;
        uint8_t hour2;

        uint8_t decimal_hour;
        uint8_t decimal_minutes;
        
        minutes1 = read_clock_minutes();
        decimal_minutes = (minutes1 / 16 * 10) + (minutes1 % 16);

        hour1 = read_clock_hour();
        decimal_hour = (hour1 / 16 * 10) + (hour1 % 16);
        
        hour2 = check_hour(decimal_hour);
        minutes2 = check_minutes(decimal_minutes);
        if (hour2 == 1 && minutes2 == 1){
            print_char(decimal_hour / 10 + '0',0x07);
            print_char(decimal_hour % 10 + '0',0x07);
            print_string(":",0x07);
            print_char(decimal_minutes / 10 + '0',0x07);
            print_char(decimal_minutes % 10 + '0',0x07);
            newline();
        }
        else{
            print_string("00:00",0x0F);
            newline();

        }
}
void year_cmd(){
        uint8_t year1;
        uint8_t decimal;
        uint8_t seculo2;
        uint8_t year2;
        uint8_t seculo3;
        uint8_t decimal_sec;
        year1 = read_clock_year();
        seculo2 = read_clock_seculo();
        if (seculo2 == 0x00){ // Hardware antigo
            seculo2 = 0x20;
        }
        decimal = (year1 / 16 * 10) + (year1 % 16);
        decimal_sec = (seculo2 / 16 * 10) + (seculo2 % 16);
        seculo3 = check_seculo(decimal_sec);
        year2 = check_ano(decimal);
        if (seculo3 == 1 && year2 == 1){
            print_char(decimal_sec / 10 + '0',0x07);
            print_char(decimal_sec % 10 + '0',0x07);
            print_char(decimal / 10 + '0',0x07);
            print_char(decimal % 10 + '0',0x07);
            newline();
        }
        else{
            print_string("2000",0x0F);
            newline();
        }
}

void date_cmd(){
        uint8_t year1;
        uint8_t day1;
        uint8_t mes1;
        uint8_t seculo1;
        uint8_t decimal_day;
        uint8_t decimal_mes;
        uint8_t decimal_year;
        uint8_t decimal_seculo;
        uint8_t dia2;
        uint8_t mes2;
        uint8_t ano2;
        seculo1 = read_clock_seculo();
        year1 = read_clock_year();
        day1 = read_clock_day();
        mes1 = read_clock_mes();
        decimal_year = (year1 / 16 * 10) + (year1 % 16);
        decimal_mes = (mes1 / 16 * 10) + (mes1 % 16);
        decimal_day = (day1 / 16 * 10) + (day1  % 16);
        decimal_seculo = (seculo1 / 16 * 10 ) + (seculo1 % 16);
        // imprimindo dia
        dia2 = check_day(decimal_day);
        mes2 = check_mes(decimal_mes);
        ano2 = check_ano(decimal_year);
        if (dia2 == 1 && mes2 == 1 && ano2 == 1){ 
            // dia
            print_char(decimal_day / 10 + '0',0x07);
            print_char(decimal_day % 10 + '0',0x07);
            print_string("/",0x07);
            // mes
            print_char(decimal_mes / 10 + '0',0x07);
            print_char(decimal_mes % 10 + '0',0x07);
            print_string("/",0x07);
            // ano e seculo
            print_char(decimal_seculo / 10 + '0',0x07);
            print_char(decimal_seculo % 10 + '0',0x07);
            print_char(decimal_year / 10 + '0',0x07);
            print_char(decimal_year % 10 + '0',0x07);        
        }
        else{
            print_string("01/01/2000",0x0F);
        }
        newline();
}
void process_command() {
    command_buffer[buffer_idx] = '\0';
    newline();

    if (str_compare(command_buffer, "cls")) {
        clear_screen2();
    } else if (str_compare(command_buffer, "help")) {
    print_string("help    - Display this help message", 0x07);
    newline();
    print_string("year    - Show current year from motherboard (RTC)", 0x07);
    newline();
    print_string("time    - Show current time from motherboard (RTC)", 0x07);
    newline();
    print_string("date    - Show full date from motherboard (RTC)", 0x07);
    newline();
    print_string("cls     - Clear the terminal screen", 0x07);
    newline();
    print_string("cr2032  - Check CMOS battery (CR2032) health status", 0x07);
    newline();
    print_string("echo    - Print a message to the console", 0x07);
    // panic -> DEBUG COMMAND
    newline();
    print_string("version - Show current PByteOS build and version", 0x07);
    newline();
    } else if (str_compare(command_buffer, "year")){
        year_cmd();
    } else if (str_compare(command_buffer, "time")){
        time_cmd();
    } else if (str_compare(command_buffer, "date")){
        date_cmd();
    } else if (str_compare(command_buffer, "cr2032")){
        char energy;
        energy = read_clock_battery();
        if (energy & 0x80){
            print_string("[ OK ] CR2032 have energy.",0x0A);
            newline();
        }
        else{
            print_string("[ x ] Battery is empty",0x0C);
            newline();
        }
    } else if (str_compare(command_buffer, "version")) {
        print_string("PByteOS 0.1.2 (Build 1903.5)", 0x0E);
        newline();
        print_string("Copyright (c) 2026 G738489.", 0x07);
        newline(); 
    } else if (str_compare(command_buffer, "panic")){
        crash_screen("Manual panic");
    } else if (starts_with(command_buffer, "echo ")) {
        char* message = command_buffer + 5;
        print_string(message, 0x07);
        newline();
    } else if (buffer_idx > 0) {
        print_string("[ x ] Invalid command.", 0x0C);
        newline();
    }

    print_string("PBYTE>> ", 0x0B);
    buffer_idx = 0;
}

void kernel_main() {

    clear_screen();
    print_string("Copyright (C) PByteOS (TM). All rights reserved [2026]", 0x0F);
    newline();
    print_string("PBYTE>> ", 0x0B);

    unsigned char last_scan = 0;
    while(1) {
        unsigned char scan;
        unsigned char status;
        __asm__ volatile("inb $0x64, %0": "=a"(status));
        if (status & 1){
            __asm__ volatile("inb $0x60, %0" : "=a"(scan));
            if (scan == 0x2A){
                is_shift_pressed = 1;
            }
            if (scan == 0x36){
                is_shift_pressed = 1;
            }
            if (scan == 0xAA || scan == 0xB6){
                is_shift_pressed = 0;
            }
            if (scan == 0x3A){
                is_caps_lock = !is_caps_lock;
            }

        }
        if (buffer_idx >= 63){
            clear_screen();
            crash_screen("Ilegal operation on command buffer");

        }
        if (cursor_pos >= 4000){
            cursor_pos = 0;
            clear_screen2();
            print_string("PBYTE>> ",0x0B);
        }
        if (scan < 128 && scan != last_scan) {
            char c = keyboard_map[scan];
            if (is_caps_lock == 1){
                if (c >= 'a' && c <= 'z'){
                    c = keyboard_map_capslock[scan];
                }
            }
            if (is_shift_pressed == 1){  
               c = keyboard_map_shift[scan];
            }


            if (c != 0) {
                if (c == '\n') {
                    process_command();
                } else if (c == '\b' && buffer_idx > 0) {
                    buffer_idx--;
                    cursor_pos -= 2;
                    VIDEO_MEM[cursor_pos] = ' ';
                    VIDEO_MEM[cursor_pos + 1] = 0x07; 
                } else if (buffer_idx < 62 && c != '\b') {
                    command_buffer[buffer_idx++] = c;
                    print_char(c, 0x0F); 
                }
            }
            last_scan = scan;
        } else if (scan >= 0x80) {
            last_scan = 0;
        }
    }
}
