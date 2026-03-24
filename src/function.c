#include "function.h"
#include "io.h"
#include "screen.h"   
#include "driver.h"   
#include "string.h"
extern char white_current_color;
extern char cyan_current_color; 
extern char red_current_color;  
extern char green_current_color;
extern char yellow_current_color;
extern char current_color;
extern char* VIDEO_MEM;
extern char command_buffer[64];
extern int buffer_idx;
extern uint32_t system_ticks;
void process_command() {
    command_buffer[buffer_idx] = '\0';
    newline();

    if (str_compare(command_buffer, "cls")) {
        clear_screen2();
    } else if (str_compare(command_buffer, "help")) {
    print_string("help    - Display this help message", current_color);
    newline();
    print_string("year    - Show current year from motherboard (RTC)", current_color);
    newline();
    print_string("time    - Show current time from motherboard (RTC)", current_color);
    newline();
    print_string("date    - Show full date from motherboard (RTC)", current_color);
    newline();
    print_string("cls     - Clear the terminal screen", current_color);
    newline();
    print_string("cr2032  - Check CMOS battery (CR2032) health status", current_color);
    newline();
    print_string("echo    - Print a message to the console", current_color);
    // panic -> DEBUG COMMAND
    newline();
    print_string("version - Show current PByteOS build and version", current_color);
    newline();
    print_string("color  - Change the global shell color (0-9)", current_color);
    newline();
    print_string("exit   - Halt the CPU for manual shutdown", current_color);
    newline();
    print_string("uptime - Show system elapsed time", current_color);
    newline();
    print_string("reboot - Restart the system", current_color);
    newline();
    print_string("mem - Shows the computer's RAM memory ", current_color);
    newline();
    } else if (str_compare(command_buffer, "year")){
        year_cmd();
    } else if (str_compare(command_buffer, "time")){
        time_cmd();
    } else if (str_compare(command_buffer, "exit")){
        clear_screen();
        print_string("System Halted. You can safely turn off the computer.", white_current_color);
        __asm__ volatile("cli; hlt");
    } else if (str_compare(command_buffer, "date")){
        date_cmd();
    } else if (str_compare(command_buffer, "reboot")){
        outb(0x64,0xFE);
        __asm__ volatile("cli; hlt");
    } else if (str_compare(command_buffer, "mem")) {
        uint32_t endereco = 0x100000;
        uint32_t mem = 1;
        while (1){
            uint32_t backup = *(volatile uint32_t*)endereco;
            *(volatile uint32_t*)endereco = 0xABCDEF12;
            uint32_t valor = *(volatile uint32_t*)endereco;
            if (valor == 0xABCDEF12){
                mem += 1;
                *(volatile uint32_t*)endereco = backup;
                endereco += 0x100000;
            }
            else{
                break;
            }
        }
        if (mem > 1024){
            if (mem == 3072){
                uint32_t gb = mem / 1024;
                print_string("RAM: 4GB",white_current_color);
                newline();                
            }
            else{
                uint32_t gb = mem / 1024;
                print_string("RAM: ",white_current_color);
                print_int(gb,white_current_color);
                print_string("GB",white_current_color);
                newline();
            }
        }
        else{
            print_string("RAM: ",white_current_color);
            print_int(mem,white_current_color);
            print_string("MB",white_current_color);
            newline();
        }
    } else if (str_compare(command_buffer, "cr2032")){
        char energy;
        energy = read_clock_battery();
        if (energy & 0x80){
            print_string("[ OK ] CR2032 have energy.",green_current_color);
            newline();
        }
        else{
            print_string("[ x ] Battery is empty",red_current_color);
            newline();
        }
    } else if (str_compare(command_buffer, "version")) {
        print_string("PByteOS 0.1.3 RC (Build 2403.1)",yellow_current_color);
        newline();
        print_string("Copyright (c) 2026 G738489.", current_color);
        newline(); 
    } else if (str_compare(command_buffer, "panic")){
        crash_screen("Manual panic");
    } else if (str_compare(command_buffer, "uptime")){
        uint32_t total_segundos = system_ticks / 100;
        uint32_t minutos = total_segundos / 60;
        uint32_t segundos_restantes = total_segundos % 60;
        print_string("Uptime: ",yellow_current_color);
        print_int(minutos,yellow_current_color);
        print_string("m",yellow_current_color);
        print_int(segundos_restantes,yellow_current_color);
        print_string("s",yellow_current_color);
        newline();
    } else if (starts_with(command_buffer, "echo ")) {
        char* message = command_buffer + 5;
        print_string(message, current_color);
        newline();
    } else if (starts_with(command_buffer, "color ")){
        char color = *(command_buffer + 6);
        if (color == '0'){
            current_color = COLOR_GREY;
            cyan_current_color = COLOR_CYAN;
            white_current_color = COLOR_WHITE;
            yellow_current_color = COLOR_YELLOW;
            green_current_color = COLOR_GREEN;
            red_current_color = COLOR_RED;
            clear_screen2();

        }
        else if (color == '1'){
            current_color = COLOR_WHITE;
            cyan_current_color = COLOR_WHITE;
            white_current_color = COLOR_WHITE;
            yellow_current_color = COLOR_WHITE;
            green_current_color = COLOR_WHITE;
            red_current_color = COLOR_WHITE;
            clear_screen2();

        }
        else if (color == '2'){
            current_color = COLOR_YELLOW;
            cyan_current_color = COLOR_YELLOW;
            white_current_color = COLOR_YELLOW;
            yellow_current_color = COLOR_YELLOW;
            green_current_color = COLOR_YELLOW;
            red_current_color = COLOR_YELLOW;
            clear_screen2();

        }
        else if (color == '3'){
            current_color = COLOR_GREEN;
            cyan_current_color = COLOR_GREEN;
            white_current_color = COLOR_GREEN;
            yellow_current_color = COLOR_GREEN;
            green_current_color = COLOR_GREEN;
            red_current_color = COLOR_GREEN;
            clear_screen2();

        }
        else if (color == '4'){
            current_color = COLOR_RED;
            cyan_current_color = COLOR_RED;
            white_current_color = COLOR_RED;
            yellow_current_color = COLOR_RED;
            green_current_color = COLOR_RED;
            red_current_color = COLOR_RED;
            clear_screen2();

        }
        else if (color == '5'){
            current_color = COLOR_CYAN;
            cyan_current_color = COLOR_CYAN;
            white_current_color = COLOR_CYAN;
            yellow_current_color = COLOR_CYAN;
            green_current_color = COLOR_CYAN;
            red_current_color = COLOR_CYAN;
            clear_screen2();

        }
        else if (color == '6'){
            current_color = COLOR_BLUE;
            cyan_current_color = COLOR_BLUE;
            white_current_color = COLOR_BLUE;
            yellow_current_color = COLOR_BLUE;
            green_current_color = COLOR_BLUE;
            red_current_color = COLOR_BLUE;
            clear_screen2();

        }
        else if (color == '7'){
            current_color = COLOR_GREY_W;
            cyan_current_color = COLOR_CYAN_W;
            white_current_color = COLOR_BLACK_W;
            yellow_current_color = COLOR_YELLOW_W;
            green_current_color = COLOR_GREEN_W;
            red_current_color = COLOR_RED_W;
            clear_screen2();

        }
        else if (color == '8'){
            current_color = 0x17;
            cyan_current_color = 0x1B;
            white_current_color = 0x1F;
            yellow_current_color = 0x1E;
            green_current_color = 0x1A;
            red_current_color = 0x1C;
            clear_screen2();

        }
        else if (color == '9'){
            current_color = COLOR_MAGENTA;
            cyan_current_color = COLOR_MAGENTA;
            white_current_color = COLOR_MAGENTA;
            yellow_current_color = COLOR_MAGENTA;
            green_current_color = COLOR_MAGENTA;
            red_current_color = COLOR_MAGENTA;
            clear_screen2();

        }
        else{
            print_string("[ x ] Invalid color (0-9).",red_current_color);
            newline();
        }
    } else if (buffer_idx > 0) {
        print_string("[ x ] Invalid command.", red_current_color);
        newline();
    }

    print_string("PBYTE>> ", cyan_current_color);
    buffer_idx = 0;
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
            print_char(decimal_hour / 10 + '0',current_color);
            print_char(decimal_hour % 10 + '0',current_color);
            print_string(":",current_color);
            print_char(decimal_minutes / 10 + '0',current_color);
            print_char(decimal_minutes % 10 + '0',current_color);
            newline();
        }
        else{
            print_string("00:00",white_current_color);
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
            print_char(decimal_sec / 10 + '0',current_color);
            print_char(decimal_sec % 10 + '0',current_color);
            print_char(decimal / 10 + '0',current_color);
            print_char(decimal % 10 + '0',current_color);
            newline();
        }
        else{
            print_string("2000",white_current_color);
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
            print_char(decimal_day / 10 + '0',current_color);
            print_char(decimal_day % 10 + '0',current_color);
            print_string("/",current_color);
            // mes
            print_char(decimal_mes / 10 + '0',current_color);
            print_char(decimal_mes % 10 + '0',current_color);
            print_string("/",current_color);
            // ano e seculo
            print_char(decimal_seculo / 10 + '0',current_color);
            print_char(decimal_seculo % 10 + '0',current_color);
            print_char(decimal_year / 10 + '0',current_color);
            print_char(decimal_year % 10 + '0',current_color);        
        }
        else{
            print_string("01/01/2000",white_current_color);
        }
        newline();
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
void timer_handler() {
    system_ticks++;
    outb(0x20, 0x20);
}
void init_timer() {
    uint16_t divisor = 11931;
    outb(0x43, 0x36);             
    outb(0x40, (uint8_t)(divisor & 0xFF)); // Low byte
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
}