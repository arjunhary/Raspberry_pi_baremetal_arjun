
ARMGNU ?= arm-none-eabi

COPS = -g -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img bootloader.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.hex

vectors.o : vectors.s 
	$(ARMGNU)-as vectors.s -o vectors.o

bootloader.o : bootloader.c *.h
	$(ARMGNU)-gcc $(COPS) -c bootloader.c -o bootloader.o

kernel.img : loader vectors.o bootloader.o *.h
	$(ARMGNU)-ld vectors.o bootloader.o -T loader -o bootloader.elf
	$(ARMGNU)-objdump -D bootloader.elf > bootloader.list
	$(ARMGNU)-objcopy bootloader.elf -O binary kernel.img
	$(ARMGNU)-objcopy bootloader.elf -O binary bootloader.bin


