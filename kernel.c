
char* VIDEO_MEM = (char*)0xB8000;

#define MAX_COLS 80
#define MAX_ROWS 25

int cursor_pos = 0;
char command_buffer[64];
int buffer_idx = 0;
int starts_with(const char* buffer, const char* prefix) {
    while(*prefix) { 
        if(*prefix != *buffer) return 0; 
        prefix++;
        buffer++;
    }
    return 1;
}
unsigned char keyboard_map[128] = {  // mapped characters

    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',

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

void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        VIDEO_MEM[i] = ' ';
        VIDEO_MEM[i+1] = 0x07;
    }
    cursor_pos = 0;
}

void newline() {
    cursor_pos = (cursor_pos / (MAX_COLS * 2) + 1) * (MAX_COLS * 2);
}

void process_command() {
    command_buffer[buffer_idx] = '\0';
    newline();

    if (str_compare(command_buffer, "cls")) {
        clear_screen();
    } else if (str_compare(command_buffer, "help")) {
        print_string("Comandos: cls, help, version, echo", 0x07);
        newline();
    } else if (str_compare(command_buffer, "version")) {
        print_string("PByteOS 0.1.1", 0x0E);
        newline();
    } else if (starts_with(command_buffer, "echo ")) {
        char* message = command_buffer + 5;
        print_string(message, 0x07);
        newline();
    } else if (buffer_idx > 0) {
        print_string("[ x ] Comando invalido.", 0x0C);
        newline();
    }

    print_string("PBYTE>> ", 0x0B);
    buffer_idx = 0;
}

void kernel_main() {
    clear_screen();
    print_string("PByteOS!", 0x0F);
    newline();
    print_string("PBYTE>> ", 0x0B);

    unsigned char last_scan = 0;
    while(1) {
        unsigned char scan;
        __asm__ volatile("inb $0x60, %0" : "=a"(scan));

        if (scan < 128 && scan != last_scan) {
            char c = keyboard_map[scan];
            if (c != 0) {
                if (c == '\n') {
                    process_command();
                } else if (c == '\b' && buffer_idx > 0) {
                    buffer_idx--;
                    cursor_pos -= 2;
                    VIDEO_MEM[cursor_pos] = ' ';
                    VIDEO_MEM[cursor_pos + 1] = 0x07;
                } else if (buffer_idx < 63 && c != '\b') {
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