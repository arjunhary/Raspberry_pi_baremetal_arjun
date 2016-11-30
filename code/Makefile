
ARMGNU ?= arm-none-eabi
vpath %.c src
vpath %.o obj
vpath %.h include

COPS = -g -Wall -Wno-trigraphs -O2 -nostdlib -ffreestanding -mcpu=cortex-a7 -mtune=arm1176jzf-s -fstack-usage

OBJS =  vectors.o arm_utility.o display.o utility.o SPI.o I2C.o Ft6206_cap_touch.o UART.o \
		Interrupts.o DMA.o synchronization.o smp.o mmu.o fifo.o bluefruit.o main.o

SRC_DIR = src/
OBJ_DIR = obj/
OUTPUT_FOLDER = output
INCLUDE_FOLDER = include

OBJS_WITH_PATH = $(addprefix $(OBJ_DIR),$(OBJS))

all : $(OUTPUT_FOLDER)/kernel.img

clean :
	rm -f ./obj/*.o
	rm -f ./*.o
	rm -f ./output/*.bin
	rm -f ./output/*.elf
	rm -f ./output/*.list
	rm -f ./output/*.img
	rm -f ./output/*.hex

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE_FOLDER)/*.h Makefile
#	@ECHO "$< -> $@"
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<	
	
$(OBJ_DIR)%.o: %.S $(INCLUDE_FOLDER)/*.h Makefile
	$(ARMGNU)-gcc -c -o $@ $<	

$(OUTPUT_FOLDER)/kernel.img : $(OBJS_WITH_PATH) loader Makefile 
#	@ECHO "$(OBJS_WITH_PATH)"
	$(ARMGNU)-ld $(OBJS_WITH_PATH) -Map ./output/display.map -T loader -o ./output/display.elf
	$(ARMGNU)-objdump -D $(OUTPUT_FOLDER)/display.elf > $(OUTPUT_FOLDER)/display.list
	$(ARMGNU)-objcopy $(OUTPUT_FOLDER)/display.elf -O binary $(OUTPUT_FOLDER)/kernel.img
	$(ARMGNU)-objcopy $(OUTPUT_FOLDER)/display.elf -O binary $(OUTPUT_FOLDER)/display.bin
	$(ARMGNU)-size --format=sysv -x $(OUTPUT_FOLDER)/display.elf
	
#obj/vectors.o : vectors.s 
#	$(ARMGNU)-as vectors.s -o ./obj/vectors.o
	
#obj/I2C.o : ./src/I2C.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/I2C.c -o ./obj/I2C.o
	
#obj/SPI.o : ./src/SPI.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/SPI.c -o ./obj/SPI.o
	
#obj/DMA.o : ./src/DMA.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/DMA.c -o ./obj/DMA.o
	
#obj/UART.o : ./src/UART.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/UART.c -o ./obj/UART.o

#obj/utility.o : ./src/utility.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/utility.c -o ./obj/utility.o

#obj/display.o : ./src/display.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/display.c -o ./obj/display.o
	
#obj/Ft6206_cap_touch.o : ./src/Ft6206_cap_touch.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/Ft6206_cap_touch.c -o ./obj/Ft6206_cap_touch.o
	
#obj/Interrupt_handler.o : ./src/Interrupt_handler.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/Interrupt_handler.c -o ./obj/Interrupt_handler.o
	
#obj/main.o : ./src/main.c ./include/*.h
#	$(ARMGNU)-gcc $(COPS) -c ./src/main.c -o ./obj/main.o
	
#output/kernel.img : loader obj/vectors.o obj/display.o obj/utility.o obj/SPI.o obj/I2C.o obj/Ft6206_cap_touch.o obj/UART.o obj/Interrupt_handler.o ./obj/DMA.o ./obj/main.o
#	$(ARMGNU)-ld ./obj/vectors.o ./obj/display.o ./obj/utility.o ./obj/SPI.o ./obj/I2C.o ./obj/Ft6206_cap_touch.o ./obj/UART.o ./obj/Interrupt_handler.o ./obj/DMA.o ./obj/main.o -T loader -o ./output/display.elf
#	$(ARMGNU)-objdump -D ./output/display.elf > ./output/display.list
#	$(ARMGNU)-objcopy ./output/display.elf -O binary ./output/kernel.img
#	$(ARMGNU)-objcopy ./output/display.elf -O binary ./output/display.bin
#	$(ARMGNU)-size --format=sysv -x ./output/display.elf

