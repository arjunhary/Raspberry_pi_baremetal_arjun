#include "./../include/I2C.h"
#include "./../include/Ft6206_cap_touch.h"
#include "./../include/utility.h"
#include "./../include/interrupts.h"
#include "./../include/SystemTimer.h"
#include "./../include/GPIO.h"

void enable_irq(void)
{
	asm volatile("mrs r0,cpsr\n\t"
				 "bic r0,r0,#0x80\n\t"
				 "msr cpsr_c,r0\n\t");
}

void enable_fiq(void)
{
	asm volatile("mrs r0,cpsr\n\t"
				 "bic r0,r0,#0x40\n\t"
				 "msr cpsr_c,r0\n\t");
}

void data_abort_handler(void)
{
	LED_fastblink();
}

int disable_system_timer_interrupt(void)
{
	Interrupt_IRQ_Disable1_Reg_t intrbasdisable;
	intrbasdisable = (Interrupt_IRQ_Disable1_Reg_t) GET32(INTERRUPT_DISABLE_IRQ1);
	intrbasdisable.mBits.irq1 = 1;
	PUT32(INTERRUPT_DISABLE_IRQ1,intrbasdisable.mAsU32);
	return 0;
}

int is_system_timer_interrupt_pending(void)
{
	Interrupt_IRQ_pending1_Reg_t irq_pending_reg;
	irq_pending_reg.mAsU32 = 0;
	irq_pending_reg.mAsU32 = GET32(INTERRUPT_IRQ_PENDING1);
	if((irq_pending_reg.mAsU32 & 0x00000002))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_gpio_interrupt_pending(void)
{
	Interrupt_IRQ_pending2_Reg_t irq_pending_reg;
	irq_pending_reg.mAsU32 = 0;
	irq_pending_reg.mAsU32 = GET32(INTERRUPT_IRQ_PENDING2);
	if(irq_pending_reg.mBits.irq49 == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}




int enable_system_timer_irq_interrupt(void)
{
	//Set the system timer register
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Set the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	//Set the Interrupt enable register
	Interrupt_IRQ_Enable1_Reg_t intrirqenable;
	intrirqenable.mAsU32 = 0;
	intrirqenable.mBits.irq1 = 1;
	PUT32(INTERRUPT_ENABLE_IRQ1,intrirqenable.mAsU32);
	
	enable_irq();
	
	return 0;
}

int enable_Ft6206_touch_screen_i2c_interrupt(void)
{
	
	//Set GPIO 24 as input. this is for interrupt
	volatile GPIO_Funtion_Select2_t GPIOFnsel2;
	GPIOFnsel2 = (GPIO_Funtion_Select2_t)GET32(GPIO_REG_GPFSEL2);
	GPIOFnsel2.mBits.FunctionSelGPIO24 = GPIO_SET_AS_INPUT;
	PUT32(GPIO_REG_GPFSEL2,GPIOFnsel2.mAsU32);
		
	//Set Async Rising edge interrupt for GPIO 24
	/*GPIO_0_31_t GPIOasync_risingedge_reg;
	GPIOasync_risingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPAREN0);
	GPIOasync_risingedge_reg.mBits.GPIO24 = 1;
	PUT32(GPIO_REG_GPAREN0,GPIOasync_risingedge_reg.mAsU32);*/
	
	//Set Async falling edge interrupt for GPIO 24
	/*GPIO_0_31_t GPIOasync_fallingedge_reg;
	GPIOasync_fallingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPAFEN0);
	GPIOasync_fallingedge_reg.mBits.GPIO24 = 1;
	PUT32(GPIO_REG_GPAFEN0,GPIOasync_fallingedge_reg.mAsU32);*/

	//Set low detect interrupt for GPIO 24
	GPIO_0_31_t GPIOasync_fallingedge_reg;
	GPIOasync_fallingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPLEN0);
	GPIOasync_fallingedge_reg.mBits.GPIO24 = 1;
	PUT32(GPIO_REG_GPLEN0,GPIOasync_fallingedge_reg.mAsU32);
	
	//Set the Interrupt enable register
	Interrupt_IRQ_Enable2_Reg_t intrirqenable;
	intrirqenable = (Interrupt_IRQ_Enable2_Reg_t) GET32(INTERRUPT_ENABLE_IRQ2);
	intrirqenable.mBits.irq49 = 1;
	PUT32(INTERRUPT_ENABLE_IRQ2,intrirqenable.mAsU32);
	
	//enable_irq();
	
	return 0;
}

int disable_Ft6206_touch_screen_i2c_interrupt(void)
{	
	//Set Async Rising edge interrupt for GPIO 24
	/*GPIO_0_31_t GPIOasync_risingedge_reg;
	GPIOasync_risingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPAREN0);
	GPIOasync_risingedge_reg.mBits.GPIO24 = 0;
	PUT32(GPIO_REG_GPAREN0,GPIOasync_risingedge_reg.mAsU32);*/
	
	//Set Async falling edge interrupt for GPIO 24
	/*GPIO_0_31_t GPIOasync_fallingedge_reg;
	GPIOasync_fallingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPAFEN0);
	GPIOasync_fallingedge_reg.mBits.GPIO24 = 0;
	PUT32(GPIO_REG_GPAFEN0,GPIOasync_fallingedge_reg.mAsU32);*/

	//Set low detect interrupt for GPIO 24
	/*GPIO_0_31_t GPIOasync_fallingedge_reg;
	GPIOasync_fallingedge_reg = (GPIO_0_31_t) GET32(GPIO_REG_GPLEN0);
	GPIOasync_fallingedge_reg.mBits.GPIO24 = 0;
	PUT32(GPIO_REG_GPLEN0,GPIOasync_fallingedge_reg.mAsU32);*/
	
	//Set the Interrupt enable register
	Interrupt_IRQ_Enable2_Reg_t intrirqenable;
	intrirqenable = (Interrupt_IRQ_Enable2_Reg_t) GET32(INTERRUPT_ENABLE_IRQ2);
	intrirqenable.mBits.irq49 = 0;
	PUT32(INTERRUPT_ENABLE_IRQ2,intrirqenable.mAsU32);
	
	//enable_irq();
	
	return 0;
}

int enable_system_timer_fiq_interrupt(void)
{
	//Set the system timer register
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Set the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	//Set the Interrupt senable register
	Interrupt_FIQ_control_Reg_t intrfiqenable;
	intrfiqenable = (Interrupt_FIQ_control_Reg_t) GET32(INTERRUPT_FIQ_CONTROL);
	intrfiqenable.mBits.FIQ_enable = 1;
	intrfiqenable.mBits.FIQ_source = 1;
	PUT32(INTERRUPT_FIQ_CONTROL,intrfiqenable.mAsU32);
	
	enable_fiq();
	
	return 0;
}

//__attribute__((interrupt("FIQ"))) 

void c_fiq_handler (void)
{
	static int led_on = 0;

	//Reset the counter match register	
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Clear the system timer match register
	volatile System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	if(led_on == 0)
	{
		LEDTurnon();
		led_on = 1;
	}
	else
	{
		LEDTurnoff();
		led_on = 0;
	}
}


//void __attribute__((interrupt("irq"))) c_irq_handler (void)
void c_irq_handler (void)
{
	static int led_on = 0;

	//Reset the counter match register	
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Clear the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	if(led_on == 0)
	{
		LEDTurnon();
		led_on = 1;
	}
	else
	{
		LEDTurnoff();
		led_on = 0;
	}
}

void polled_sys_timer_interrupt_handler(void)
{
	static int led_on =0;
	if(is_system_timer_interrupt_pending() == 1)
	{
		//Reset the counter match register
		unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
		PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
		
		//Clear the system timer match register
		System_timer_control_Reg_t systimctrl;
		systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
		systimctrl.mBits.System_timer1_match = 1;
		PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
		
		if(led_on == 0)
		{
			LEDTurnon();
			led_on = 1;
		}
		else
		{
			LEDTurnoff();
			led_on = 0;
		}
	}
}




void  polled_Ft6206_irq_handler (void)
{
	volatile GPIO_0_31_t gpio_event_detect_1;
	GPIO_0_31_t gpio_event_clear;
	gpio_event_clear.mAsU32 = 0;
	unsigned int level_detect = 0;
	gpio_event_detect_1.mAsU32 = 0;
	if(is_gpio_interrupt_pending())
	{	
		gpio_event_detect_1 = (GPIO_0_31_t)GET32(GPIO_REG_GPEDS0);
		if(gpio_event_detect_1.mBits.GPIO24 == 1)
		{
			gpio_event_clear.mBits.GPIO24 = 1;
			PUT32(GPIO_REG_GPEDS0,gpio_event_clear.mAsU32);
			level_detect = (int)GET32(GPIO_REG_GPLEV0);
			FT6206_get_num_touches();
		}
	}
}