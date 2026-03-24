# PByteOS

### What is PByteOS?
A custom x86 hobby OS. Simple, fast, and practical. From bootloader to kernel, it is all bare metal.

### Is it 32-bit or 16-bit?
**32-bit (Protected Mode)**. It has full access to 4GB of RAM and uses a modern flat memory layout.

### How do I compile it?
Windows:
```
nasm -f elf32 src/boot.asm -o boot.o
gcc -m32 -c src/kernel.c -o kernel.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/screen.c -o screen.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/keyboard.c -o keyboard.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/function.c -o function.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/idt.c -o idt.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/io.c -o io.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/string.c -o string.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
gcc -m32 -c src/driver.c -o driver.o -Iheaders -ffreestanding -O2 -nostdlib -fno-stack-protector -fno-leading-underscore
ld -m i386pe -T linker.ld boot.o kernel.o screen.o keyboard.o function.o idt.o io.o string.o driver.o -o kernel.tmp
objcopy -O elf32-i386 kernel.tmp kernel.bin
qemu-system-i386 -kernel kernel.bin
```
### Will it continue to be a terminal?
Probably not. My ultimate goal is to evolve PByteOS from a CLI to a full graphical user interface (GUI).
