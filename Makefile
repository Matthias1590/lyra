qemu: bin/disk.img
	qemu-system-i386 -drive file=bin/disk.img,format=raw

bin/disk.img: bin/bootloader.bin bin/kernel.bin
	dd if=/dev/zero of=bin/disk.img bs=512 count=2880
	dd if=bin/bootloader.bin of=bin/disk.img bs=512 count=1 conv=notrunc
	dd if=bin/kernel.bin of=bin/disk.img bs=512 seek=1 count=32 conv=notrunc

bin/bootloader.bin: src/bootloader.asm
	nasm -f bin -o bin/bootloader.bin src/bootloader.asm

bin/kernel.bin: src/kernel.c
	gcc -fno-pie -m32 -ffreestanding -c src/kernel.c -o bin/kernel.o
	ld -m elf_i386 -o bin/kernel.bin -Ttext 0x1000 bin/kernel.o --oformat binary

	# limit kernel size to 32 sectors (16KB), can be changed
	# by changing the dd commands above and bootloader code
	@size=`wc -c < bin/kernel.bin`; \
	if [ $$size -gt 16384 ]; then \
		echo "Kernel is too large: $$size bytes"; \
		exit 1; \
	fi

clean:
	rm -f bin/*.bin bin/*.o bin/*.img
