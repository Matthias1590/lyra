qemu: bin/lyra.img
	qemu-system-i386 -drive file=bin/lyra.img,format=raw

clean:
	rm -f bin/*

# Image
bin/lyra.img: bin/bootloader.bin bin/kernel.bin
	dd if=/dev/zero          of=bin/lyra.img bs=32M count=1
	dd if=bin/bootloader.bin of=bin/lyra.img bs=512 count=1  conv=notrunc
	dd if=bin/kernel.bin     of=bin/lyra.img bs=512 count=32 conv=notrunc seek=1

# Bootloader
bin/bootloader.bin: src/bootloader.asm
	nasm -f bin -o bin/bootloader.bin src/bootloader.asm

# Kernel
bin/kernel.bin: bin/kernel.o
	ld -m elf_i386 -o bin/kernel.bin -Ttext 0x1000 bin/kernel.o --oformat binary

bin/kernel.o: src/kernel.c
	gcc -fno-pie -m32 -ffreestanding -c src/kernel.c -o bin/kernel.o
