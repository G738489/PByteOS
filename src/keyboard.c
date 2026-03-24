#include "keyboard.h"
#include "io.h"
#include "idt.h"
#include "function.h"
#include "screen.h"
extern char* VIDEO_MEM;
extern char white_current_color;
int shift_pressed = 0;
int caps_lock = 0;

char command_buffer[64];
int buffer_idx = 0;
unsigned char keyboard_map[128] = {  // mapped characters

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',

};

unsigned char keyboard_map_shift[128] = {  // mapped characters

    0,  27, '!', '@', '#', '$', '%', '6', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',

};

unsigned char keyboard_map_capslock[128] = {  // mapped characters

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ',

};

void keyboard_handler() {
    uint8_t scancode = inb(0x60); 

    if (scancode & 0x80) {
        uint8_t release_code = scancode & 0x7F;
        if (release_code == 42 || release_code == 54) { 
            shift_pressed = 0;
        }
        outb(0x20, 0x20); 
        return;
    }

    switch (scancode) {
        case 42: 
        case 54: 
            shift_pressed = 1;
            break;
        case 58: 
            caps_lock = !caps_lock;
            break;
        case 28: 
            command_buffer[buffer_idx] = '\0'; 
            process_command();              
            buffer_idx = 0;                 
            break;
        case 14: 
            if (buffer_idx > 0) {
                buffer_idx--;
                if (cursor_pos > 0) {
                    cursor_pos -= 2; 
                    VIDEO_MEM[cursor_pos] = ' ';
                    VIDEO_MEM[cursor_pos + 1] = white_current_color;
                }
            }
            break;
        default:
            if (scancode < 128) {
                char ascii;
                if (shift_pressed) {
                    ascii = keyboard_map_shift[scancode];
                } else if (caps_lock) {
                    ascii = keyboard_map_capslock[scancode];
                } else {
                    ascii = keyboard_map[scancode];
                }

                if (ascii != 0 && buffer_idx < 62) {
                    print_char(ascii, white_current_color); 
                    command_buffer[buffer_idx++] = ascii; 
                }
            }
            break;
    }

    outb(0x20, 0x20); 
}