
ARMGNU ?= arm-none-eabi

COPS = -g -Wall -Wno-trigraphs -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a7 -mtune=arm1176jzf-s -mhard-float 

all : output/kernel.img

clean :
	rm -f ./obj/*.o
	rm -f ./output/*.bin
	rm -f ./output/*.elf
	rm -f ./output/*.list
	rm -f ./output/*.img
	rm -f ./output/*.hex

obj/vectors.o : vectors.s 
	$(ARMGNU)-as vectors.s -o ./obj/vectors.o
	
obj/I2C.o : ./src/I2C.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/I2C.c -o ./obj/I2C.o
	
obj/SPI.o : ./src/SPI.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/SPI.c -o ./obj/SPI.o
	
obj/DMA.o : ./src/DMA.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/DMA.c -o ./obj/DMA.o
	
obj/UART.o : ./src/UART.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/UART.c -o ./obj/UART.o

obj/utility.o : ./src/utility.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/utility.c -o ./obj/utility.o

obj/display.o : ./src/display.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/display.c -o ./obj/display.o
	
obj/Ft6206_cap_touch.o : ./src/Ft6206_cap_touch.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/Ft6206_cap_touch.c -o ./obj/Ft6206_cap_touch.o
	
obj/Interrupt_handler.o : ./src/Interrupt_handler.c ./include/*.h
	$(ARMGNU)-gcc $(COPS) -c ./src/Interrupt_handler.c -o ./obj/Interrupt_handler.o
	
output/kernel.img : loader obj/vectors.o obj/display.o obj/utility.o obj/SPI.o obj/I2C.o obj/Ft6206_cap_touch.o obj/UART.o obj/Interrupt_handler.o ./obj/DMA.o
	$(ARMGNU)-ld ./obj/vectors.o ./obj/display.o ./obj/utility.o ./obj/SPI.o ./obj/I2C.o ./obj/Ft6206_cap_touch.o ./obj/UART.o ./obj/Interrupt_handler.o ./obj/DMA.o -T loader -o ./output/display.elf
	$(ARMGNU)-objdump -D ./output/display.elf > ./output/display.list
	$(ARMGNU)-objcopy ./output/display.elf -O binary ./output/kernel.img
	$(ARMGNU)-objcopy ./output/display.elf -O binary ./output/display.bin
	$(ARMGNU)-size --format=sysv -x ./output/display.elf

