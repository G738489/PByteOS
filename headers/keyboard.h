#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>
extern unsigned char keyboard_map[128];
extern unsigned char keyboard_map_shift[128];
extern unsigned char keyboard_map_capslock[128];

extern void keyboard_handler_asm();
void keyboard_handler();

#endif