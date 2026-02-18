# PByteOS

### What is PByteOS?
A custom x86 hobby OS. Simple, fast, and practical. From bootloader to kernel, it is all bare metal.

### Is it 32-bit or 16-bit?
**32-bit (Protected Mode)**. It has full access to 4GB of RAM and uses a modern flat memory layout.

### How do I compile it?
To build and run PByteOS, use the following commands in your terminal:

```
nasm -f elf32 boot.asm -o boot.o
gcc -m32 -c kernel.c -o kernel.o -ffreestanding -O2 -nostdlib -fno-stack-protector
ld -m elf_i386 -n -o kernel.bin -T linker.ld boot.o kernel.o
qemu-system-i386 -kernel kernel.bin
```
### Will it continue to be a terminal?
Probably not. My ultimate goal is to evolve PByteOS from a CLI to a full graphical user interface (GUI).
