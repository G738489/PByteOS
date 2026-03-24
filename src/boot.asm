
bits 32

section .multiboot
align 4
    dd 0x1BADB002              ; Magic number
    dd 0x00                    
    dd -(0x1BADB002 + 0x00)    

section .text
global start
global timer_handler_asm        
global keyboard_handler_asm

extern kernel_main
extern timer_handler            
extern keyboard_handler

start:

    mov esp, stack_top

    call kernel_main

.hang:
    hlt
    jmp .hang


timer_handler_asm:
    pushad                     
    call timer_handler          
    popad                      
    iretd                      

keyboard_handler_asm:
    pushad
    call keyboard_handler
    popad
    iretd
    
section .bss
align 16
stack_bottom:
    resb 16384                 
stack_top:                     