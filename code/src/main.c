#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/SPI.h"
#include "./../include/display.h"
#include "./../include/I2C.h"
#include "./../include/Ft6206_cap_touch.h"
#include "./../include/DMA.h"
#include "./../include/Interrupts.h"
#include "./../include/SystemTimer.h"
#include "./../include/sysconfig.h"
#include "./../include/smp.h"
#include "./../include/mmu.h"
#include "./../include/fifo.h"
#include "./../include/synchronization.h"
#include "./../include/bluefruit.h"

#define CMD_BUFFER_SIZE 128

extern int core_to_execute;
extern fifo_t fifo;

void init_all(void)
{
	//Initialize variable
	core_to_execute = 0;
	
	//time_sleep(1000);
	//LED ,UART and JTAG initialization
	LEDInit();
	UARTInit();
	arm_jtag_init();
			
	//Initialize the MMU and caches
	mmu_init();
		
	//Initialize the critical section semaphore
	init_critical_section();
	
	//I2C Init
	i2c_init();
	//SPI Init
	spi_init();	
	time_sleep(1000);
	//Initialize the TFT display
	//ili9341_init_tft_display();
	//ili9341_set_screen_rotation(ROTATION_0,RGB);
	
	//init the heap
	malloc_init();
	
	uart_printf("\n***************************************************************");
}	
	
int notmain(void)
{	char cmd[CMD_BUFFER_SIZE];
	int cmd_length = 0,i=0;
	init_all();
	//Tests
	//string_tests();
	//FT6206_tests();
	//memory_alignment_tests();
	//sizeof_tests();
	//factorial(50);
	//memory_tests();
	//uart_tests();
	//cache_test();
	//print_arm_registers();
	//print_heap_size();
	//uart_printf_tests();
	//ili9341_tests();
	//DMA_test();
	//fifo_init(&fifo,MAX_FIF0_SIZE);
	//fifo_producer(&fifo);
	//fifo_consumer(&fifo);
	//time_sleep(2000);
	//enable_Ft6206_touch_screen_i2c_interrupt();
	//enable_system_timer_fiq_interrupt();
	//enable_system_timer_irq_interrupt();
	//print_coreid();
	//factorial(100);
	//ex_tests();
	//malloc_tests();
	bluefruit_init();
	
	while(1)
	{
		uart_get_command(cmd, CMD_BUFFER_SIZE,&cmd_length);
		serial_command_parse(cmd,cmd_length);
	}
		
	//multicore_init();
	//core0_idleloop();
	//core0_function();
	//polled_sys_timer_interrupt_handler();
	//polled_Ft6206_irq_handler();
	uart_printf("\nReached End of main");
	//Should never reach here
	SET_BREAKPOINT();
	return 0;	
}

