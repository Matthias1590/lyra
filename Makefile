GCC := gcc -I src -fno-pie -m32 -ffreestanding
NASM := nasm -f bin
LD := ld
DD := dd

DRIVERS_SRC := $(wildcard src/drivers/*.c)
DRIVERS_OBJ := $(patsubst src/drivers/%.c, bin/drivers/%.o, $(DRIVERS_SRC))

qemu: bin/lyra.img
	qemu-system-i386 -drive file=bin/lyra.img,format=raw

clean:
	rm -rf bin

dir(%):
	mkdir -p $*

# Image
bin/lyra.img: dir(bin) bin/bootloader.bin bin/kernel.bin
	$(DD) of=$@ if=/dev/zero          bs=32M count=1
	$(DD) of=$@ if=bin/bootloader.bin bs=512 count=1  conv=notrunc
	$(DD) of=$@ if=bin/kernel.bin     bs=512 count=32 conv=notrunc seek=1

# Bootloader
bin/bootloader.bin: dir(bin) src/bootloader.asm
	$(NASM) -o $@ src/bootloader.asm

# Kernel
bin/kernel.bin: dir(bin) src/kernel.ld bin/kernel.o $(DRIVERS_OBJ)
	$(LD) -T src/kernel.ld -o $@ bin/kernel.o $(DRIVERS_OBJ)

bin/kernel.o: dir(bin) src/kernel.c
	$(GCC) -o $@ -c src/kernel.c

# Drivers
bin/drivers/%.o: dir(bin/drivers) src/drivers/%.c
	$(GCC) -o $@ -c src/drivers/$*.c
