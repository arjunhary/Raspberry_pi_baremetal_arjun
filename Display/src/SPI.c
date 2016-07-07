#include "./../include/SPI.h"
#include "./../include/GPIO.h"
#include "./../include/utility.h"

extern unsigned int start_time;

int spi_sendreadcommand(unsigned char command, int len,unsigned char* buf)
{
	int i=0;
	unsigned char dummy_value = 0xff;
	
	//Set the control register . Clear the fifo bits and set TA =1
	start_spi_transfer();
	
	//Clear GPI0 25.Sending a command
	set_DC_low_for_command();
	
	//Wait till the TX buffer has space to send a byte
	wait_till_tx_fifo_not_full();
	
	//Put the command in the fifo
	PUT32(SPI_TX_RX_FIFO,command);
	
	//Wait for transfer to be complete
	wait_for_transfer_complete();
	
	//Simply read the FIFO to ermpty
	GET32(SPI_TX_RX_FIFO);
	
	//Set GPIO25
	set_DC_high_for_data();
	
	for(i =0; i<len; i++)
	{	

		PUT32(SPI_TX_RX_FIFO,dummy_value);		
			
		//Wait till there is data
		wait_till_rx_fifo_has_data();
					
		//Get the data from the fifo
		buf[i] = GET32(SPI_TX_RX_FIFO);
	}
	
	
	
	//Set TA = 0 and clear the fifo
	stop_spi_transfer();
	
	/*spi_sendbytes(1,&dummy_value);
	
	for(i =0; i<len; i++)
	{		
		spi_getbytes(&buf[i],len);
	}*/
		
	return 0;
}


void spi_init(void)
{
	//Alternate fn 0 for GPIO 7,8,9,10,11
	GPIO_Funtion_Select0_t gpiofnsel0;
	gpiofnsel0 = (GPIO_Funtion_Select0_t) GET32(GPIO_REG_GPFSEL0);
	gpiofnsel0.mBits.FunctionSelGPIO7 = GPIO_SET_alternate_function_0;
	gpiofnsel0.mBits.FunctionSelGPIO8 = GPIO_SET_alternate_function_0;
	gpiofnsel0.mBits.FunctionSelGPIO9 = GPIO_SET_alternate_function_0;
	PUT32(GPIO_REG_GPFSEL0,gpiofnsel0.mAsU32);
	
	GPIO_Funtion_Select1_t gpiofnsel1;
	gpiofnsel1 = (GPIO_Funtion_Select1_t) GET32(GPIO_REG_GPFSEL1);
	gpiofnsel1.mBits.FunctionSelGPIO10 = GPIO_SET_alternate_function_0;
	gpiofnsel1.mBits.FunctionSelGPIO11 = GPIO_SET_alternate_function_0;
	PUT32(GPIO_REG_GPFSEL1,gpiofnsel1.mAsU32);
	
	//Configure gpio 24,25 as output. 25 is for data/command selection. 24 is for Interrupt
	volatile GPIO_Funtion_Select2_t GPIOFnsel2;
	GPIOFnsel2 = (GPIO_Funtion_Select2_t)GET32(GPIO_REG_GPFSEL2);
	GPIOFnsel2.mBits.FunctionSelGPIO25 = GPIO_SET_AS_OUTPUT;
	PUT32(GPIO_REG_GPFSEL2,GPIOFnsel2.mAsU32);
		
	//Set clk speed
	SPI_clk_Reg_t spiclkspeed;
	spiclkspeed = (SPI_clk_Reg_t)GET32(SPI_CLK_DIVIDER);
	spiclkspeed.mBits.clk_div = BCM2835_SPI_CLOCK_DIVIDER_4;
	PUT32(SPI_CLK_DIVIDER,spiclkspeed.mAsU32);
	
	//Set the SPI control register
	SPI_control_Reg_t spictrlreg;
	spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
	spictrlreg.mAsU32 = 0x00000030;
	PUT32(SPI_CONTROL_STATUS_REGISTER,spictrlreg.mAsU32);	
}



void set_DC_low_for_command(void)
{
	//Clear GPI0 25.Sending a command
	volatile GPIO_Output_Clear_Register0_t GPIOClearReg0;
	GPIOClearReg0 = (GPIO_Output_Clear_Register0_t)GET32(GPIO_REG_GPCLR0);
	GPIOClearReg0.mBits.ClearGPIO25 = 1;
	PUT32(GPIO_REG_GPCLR0,GPIOClearReg0.mAsU32);
}

void set_DC_high_for_data(void)
{
	//Set GPIO25
	volatile GPIO_Output_Set_Register0_t GPIOsetReg0;
	GPIOsetReg0 = (GPIO_Output_Set_Register0_t)GET32(GPIO_REG_GPSET0);
	GPIOsetReg0.mBits.SetGPIO25 = 1;
	PUT32(GPIO_REG_GPSET0,GPIOsetReg0.mAsU32);
}

void stop_spi_transfer(void)
{
	//Set TA = 0 and clear the fifo
	volatile SPI_control_Reg_t spictrlreg;
	spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
	spictrlreg.mAsU32 = 0x00000000;
	PUT32(SPI_CONTROL_STATUS_REGISTER,spictrlreg.mAsU32);
}


void spi_dma_enable_and_adcs(void)
{
	volatile SPI_control_Reg_t spictrlreg;
	spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
	spictrlreg.mBits.dma_enable = 1;
	spictrlreg.mBits.auto_desel_cs =0;
	spictrlreg.mBits.transfer_active = 1;
	PUT32(SPI_CONTROL_STATUS_REGISTER,spictrlreg.mAsU32);
}

void spi_dma_disable_and_adcs(void)
{
	volatile SPI_control_Reg_t spictrlreg;
	spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
	spictrlreg.mBits.dma_enable = 0;
	spictrlreg.mBits.auto_desel_cs =0;
	PUT32(SPI_CONTROL_STATUS_REGISTER,spictrlreg.mAsU32);
}

void start_spi_transfer(void)
{
	//Set the control register . Clear the fifo bits and set TA =1. Assumes chip select 0
	volatile SPI_control_Reg_t spictrlreg;
	spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
	spictrlreg.mAsU32 = 0x000000B0;
	PUT32(SPI_CONTROL_STATUS_REGISTER,spictrlreg.mAsU32);
}

void wait_for_transfer_complete()
{
	volatile SPI_control_Reg_t spictrlreg;
	while(1)
	{
		spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
		if(spictrlreg.mBits.transfer_done == 1)
		{
			break;
		}
	}	
}

void wait_till_rx_fifo_has_data(void)
{
	volatile SPI_control_Reg_t spictrlreg;
	//Wait till the RX buffer has data
	while(1)
	{
		spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
		if(spictrlreg.mBits.rx_fifo_empty == 1)
		{
			break;
		}
	}
}


void wait_till_tx_fifo_not_full(void)
{
	volatile SPI_control_Reg_t spictrlreg;
	while(1)
	{
		spictrlreg = (SPI_control_Reg_t) GET32(SPI_CONTROL_STATUS_REGISTER);
		if(spictrlreg.mBits.tx_fifo_full == 1)
		{
			break;
		}
	}		
}

int spi_sendcommand(unsigned char command, int len,unsigned char* buf)
{
	int i=0;
	
	//Clear GPI0 25.Sending a command
	set_DC_low_for_command();
	
	//Set the control register . Clear the fifo bits and set TA =1
	start_spi_transfer();
	
	//Wait till the TX buffer has space to send a byte
	//wait_till_tx_fifo_not_full();

	//Put the data in the fifo
	PUT32(SPI_TX_RX_FIFO,(unsigned int)command);
	
	//Wait for transfer to be complete
	void wait_for_transfer_complete();	
	
	//Set TA = 0 and clear the fifo
	stop_spi_transfer();
	
	if(len > 0)
	{		
		//Set the control register . Clear the fifo bits and set TA =1
		start_spi_transfer();
		
		//Set GPIO25
		set_DC_high_for_data();
		
		for(i =0; i<len; i++)
		{
			//Wait till the TX buffer has space to send a byte			
			wait_till_tx_fifo_not_full();
			PUT32(SPI_TX_RX_FIFO,(unsigned int)(buf[i]));
		}
		
		//Wait for transfer to be complete
		void wait_for_transfer_complete();

		//Set TA = 0 and clear the fifo
		stop_spi_transfer();
	}
	return 0;
}

int toggle = -1;

int spi_sendbytes(unsigned int number_of_bytes,unsigned char*ptr)
{
	int i =0;
	
	//Set GPIO25
	set_DC_high_for_data();
		
	//Set the control register . Clear the fifo bits and set TA =1
	start_spi_transfer();
	
	for(i =0; i<number_of_bytes; i++)
	{	
		GET32(SPI_TX_RX_FIFO);
		
		//Wait till the TX buffer has space to send a byte
		//wait_till_tx_fifo_not_full();		
		
		PUT32(SPI_TX_RX_FIFO,(ptr[i]));
	}			
	//Wait for transfer to be complete
	void wait_for_transfer_complete();

	//Set TA = 0 and clear the fifo
	stop_spi_transfer();

	return 0;
}


int spi_getbytes(unsigned char*ptr, int number_of_bytes)
{
	int i =0;
	unsigned int data = 0;
	
	//Set the control register . Clear the fifo bits and set TA =1
	start_spi_transfer();
		
	//Set GPIO25
	set_DC_high_for_data();
		
	//Wait till there is data
	wait_till_rx_fifo_has_data();
				
	for(i=0;i <number_of_bytes;i++)
	{	
		//Get the data from the fifo
		data = GET32(SPI_TX_RX_FIFO);
		ptr[i] = (char)data;
	}
	
	//Wait for transfer to be complete
	void wait_for_transfer_complete();

	//Set TA = 0 and clear the fifo
	stop_spi_transfer();
	
	return 0;
}