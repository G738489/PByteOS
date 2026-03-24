// PByteOS(TM) 0.1.3 Beta Preview
// Copyright (c) 2026 G738489.
// Licensed under the MIT License.

#include <stdint.h>
#include "idt.h"       
#include "screen.h"    
#include "driver.h"    
#include "function.h"  

extern uint32_t system_ticks;
extern char command_buffer[64];
extern int buffer_idx;
extern char white_current_color;
extern char cyan_current_color;

void kernel_main() {
    init_timer();
    idt_init();
    clear_screen();
    print_string("Soli Deo Gloria",white_current_color);
    newline();
    print_string("Copyright (C) PByteOS (TM). All rights reserved [2026]", white_current_color);
    newline();

    print_string("PBYTE>> ", cyan_current_color);

    while(1) {
        __asm__ volatile("hlt");
    }
}