#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/SPI.h"
#include "./../include/display.h"
#include "./../include/I2C.h"
#include "./../include/Ft6206_cap_touch.h"
#include "./../include/DMA.h"
#include "./../include/Interrupts.h"
#include "./../include/SystemTimer.h"

void init_all(void)
{
	time_sleep(1000);
	LEDInit();
	UARTInit();
	uart_tests();
	time_sleep(1000);
	//Enable Instruction cache
	enable_instruction_cache();
	//Enable Data and unified cache
	//enable_data_and_unified_cache();
	//enable_L2_cache();
	arm_jtag_init();
	i2c_init();
	spi_init();	
	time_sleep(1000);
	uart_tests();
	ili9341_init_tft_display();
	ili9341_set_screen_rotation(ROTATION_0,RGB);
}	
	
int notmain(void)
{	
	init_all();
	//Tests
	//FT6206_tests();
	ili9341_tests();
	/*DMA_test();
	ili9341_fill_color(10,10,20,20,COLOR_BLACK);
	ili9341_fill_color_dma(30,30,40,40,COLOR_BLACK);*/
	
	time_sleep(2000);
	//enable_Ft6206_touch_screen_i2c_interrupt();
	//enable_system_timer_fiq_interrupt();
	enable_system_timer_irq_interrupt();
		
	while(1)
	{
		dummy(0);
		//polled_sys_timer_interrupt_handler();
		//polled_Ft6206_irq_handler();
	}
	SET_BREAKPOINT();
	return 0;
	
}