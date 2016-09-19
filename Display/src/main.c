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


extern int core_to_execute;

void init_all(void)
{
	//Initialize variable
	core_to_execute = 0;
	//LED ,UART and JTAG initialization
	time_sleep(1000);
	LEDInit();
	UARTInit();
	arm_jtag_init();
	
	//Initialize the MMU and caches
	mmu_init();

	//I2C Init
	i2c_init();
	//SPI Init
	spi_init();	
	time_sleep(1000);
	//Initialize the TFT display
	ili9341_init_tft_display();
	ili9341_set_screen_rotation(ROTATION_0,RGB);
	
	uart_printf("\n***************************************************************");
}	
	
int notmain(void)
{	
	init_all();
	//Tests
	//string_tests();
	//FT6206_tests();
	//memory_alignment_tests();
	//sizeof_tests();
	//factorial(50);
	memory_tests();
	//uart_tests();
	//cache_test();
	//print_arm_registers();
	//print_heap_size();
	//uart_printf_tests();
	ili9341_tests();
	//DMA_test();
	
	//time_sleep(2000);
	//enable_Ft6206_touch_screen_i2c_interrupt();
	//enable_system_timer_fiq_interrupt();
	//enable_system_timer_irq_interrupt();
	//print_coreid();
	//factorial(100);
		
	multicore_init();
	//core0_idleloop();
	core0_function();
	//polled_sys_timer_interrupt_handler();
	//polled_Ft6206_irq_handler();

	//Should never reach here
	SET_BREAKPOINT();
	return 0;	
}

