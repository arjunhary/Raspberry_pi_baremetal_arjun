#include "./../include/I2C.h"
#include "./../include/GPIO.h"
#include "./../include/utility.h"
#include "./../include/SystemTimer.h"



extern void PUT32 ( unsigned int addr, unsigned int value);
extern unsigned int GET32 ( unsigned int );

char i2c_status[128];


void i2c_scan(void)
{
	int status_index = 0;
	unsigned char i2c_start_addr = 0x38;
	unsigned char i2c_stop_addr  = 0x39;
	unsigned char index = 0;
	memset((char*)i2c_status,0x00,128);
	
	for(index = i2c_start_addr; index < i2c_stop_addr; index++)
	{
		clear_status_register();
		clear_fifos();
		i2c_set_slave_addr(index);
		i2c_set_data_len(0);
		start_write_transfer();
		wait_till_transfer_done();
		if(check_for_i2c_error() == 0)
		{
			i2c_status[status_index] = index;
			status_index++;
		}
		i2c_stop_transfer();
		time_sleep(100);
	}
}

void i2c_init(void)
{
	//Alternate fn 0 for GPIO 2,3
	GPIO_Funtion_Select0_t gpiofnsel0;
	gpiofnsel0 = (GPIO_Funtion_Select0_t) GET32(GPIO_REG_GPFSEL0);
	gpiofnsel0.mBits.FunctionSelGPIO2 = GPIO_SET_alternate_function_0;
	gpiofnsel0.mBits.FunctionSelGPIO3 = GPIO_SET_alternate_function_0;
	PUT32(GPIO_REG_GPFSEL0,gpiofnsel0.mAsU32);
	
	clear_fifos();
		
	i2c_set_clk_div(I2C_CLK_DIV);
	
	//i2c_enable();
}

int check_for_i2c_error(void)
{
	volatile BSC1_I2C_status_Reg_t i2cstatusreg;
	i2cstatusreg.mAsU32 = GET32(BSC1_I2C_STATUS_REG);
	if((i2cstatusreg.mBits.NAK_error == 1) || (i2cstatusreg.mBits.clk_stretch_timeout == 1))
	{
		return -1;
	}
	return 0;
}

void i2c_enable(void)
{
	BSC1_I2C_control_Reg_t i2cctrlreg;
	i2cctrlreg = (BSC1_I2C_control_Reg_t) GET32(BSC1_I2C_CONTROL_REG);
	i2cctrlreg.mBits.clear_fifo = 3;
	i2cctrlreg.mBits.I2C_enable = 1;
	PUT32(BSC1_I2C_CONTROL_REG,i2cctrlreg.mAsU32);
}

void start_write_transfer()
{
	BSC1_I2C_control_Reg_t i2cctrlreg;
	i2cctrlreg = (BSC1_I2C_control_Reg_t) GET32(BSC1_I2C_CONTROL_REG);
	i2cctrlreg.mBits.read_write_transfer = 0;
	i2cctrlreg.mBits.clear_fifo = 3;
	i2cctrlreg.mBits.start_transfer =1 ;
	i2cctrlreg.mBits.I2C_enable = 1;
	PUT32(BSC1_I2C_CONTROL_REG,i2cctrlreg.mAsU32);
}

void start_read_transfer()
{
	BSC1_I2C_control_Reg_t i2cctrlreg;
	i2cctrlreg.mAsU32 = 0;
	i2cctrlreg = (BSC1_I2C_control_Reg_t) GET32(BSC1_I2C_CONTROL_REG);
	i2cctrlreg.mBits.read_write_transfer = 1;
	i2cctrlreg.mBits.clear_fifo = 3;
	i2cctrlreg.mBits.start_transfer =1 ;
	i2cctrlreg.mBits.I2C_enable = 1;
	PUT32(BSC1_I2C_CONTROL_REG,i2cctrlreg.mAsU32);
}

void set_data_delay(unsigned short Rising_edge_delay, unsigned short falling_edge_delay)
{
	BSC1_I2C_data_delay_Reg_t datadelayreg;
	datadelayreg = (BSC1_I2C_data_delay_Reg_t) GET32(BSC1_I2C_DATA_DELAY_REG);
	datadelayreg.mBits.Rising_edge_delay	=	Rising_edge_delay;
	datadelayreg.mBits.falling_edge_delay	=	falling_edge_delay;
	PUT32(BSC1_I2C_DATA_DELAY_REG,datadelayreg.mAsU32);	
}


void clear_fifos(void)
{
	BSC1_I2C_control_Reg_t i2cctrlreg;
	i2cctrlreg = (BSC1_I2C_control_Reg_t) GET32(BSC1_I2C_CONTROL_REG);
	i2cctrlreg.mBits.clear_fifo = 3;
	PUT32(BSC1_I2C_CONTROL_REG,i2cctrlreg.mAsU32);	
}

void clear_status_register(void)
{
	BSC1_I2C_status_Reg_t i2cstatusreg;
	i2cstatusreg.mAsU32 = 0;
	i2cstatusreg.mBits.transfer_done 		= 1;
	i2cstatusreg.mBits.NAK_error 			= 1;
	i2cstatusreg.mBits.clk_stretch_timeout	= 1;
	PUT32(BSC1_I2C_STATUS_REG,i2cstatusreg.mAsU32);
}

void i2c_set_clk_div(unsigned int clk_div)
{	
	BSC1_I2C_clk_Reg_t i2cclkreg;
	i2cclkreg = (BSC1_I2C_clk_Reg_t) GET32(BSC1_I2C_CLK_DIV_REG);
	i2cclkreg.mBits.clk_div = clk_div;
	PUT32(BSC1_I2C_CLK_DIV_REG,i2cclkreg.mAsU32);	
}

void i2c_set_data_len(unsigned short data_length)
{	
	BSC1_I2C_dlen_Reg_t i2cdlenreg;
	i2cdlenreg = (BSC1_I2C_dlen_Reg_t) GET32(BSC1_I2C_DLEN_REG);
	i2cdlenreg.mBits.Data_length = data_length;
	PUT32(BSC1_I2C_DLEN_REG,i2cdlenreg.mAsU32);	
}

void i2c_get_data_len(unsigned short* data_length)
{	
	*data_length = (unsigned short)GET32(BSC1_I2C_DLEN_REG);	
}

void i2c_set_slave_addr(unsigned char slave_addr)
{	
	BSC1_I2C_slave_addr_Reg_t i2cslavereg;
	i2cslavereg = (BSC1_I2C_slave_addr_Reg_t) GET32(BSC1_I2C_SLAVE_ADDR_REG);
	i2cslavereg.mBits.Slave_address = slave_addr;
	PUT32(BSC1_I2C_SLAVE_ADDR_REG,i2cslavereg.mAsU32);	
}

void i2c_put_data_byte(char data)
{	
	PUT32(BSC1_I2C_DATA_FIFO_REG,(int)data);	
}

void i2c_get_data_byte(volatile char* data)
{
	*data = (char)GET32(BSC1_I2C_DATA_FIFO_REG);
}

void i2c_stop_transfer(void)
{
	BSC1_I2C_control_Reg_t i2cctrlreg;
	i2cctrlreg = (BSC1_I2C_control_Reg_t) GET32(BSC1_I2C_CONTROL_REG);
	i2cctrlreg.mBits.clear_fifo = 3;
	i2cctrlreg.mBits.start_transfer = 0;
	i2cctrlreg.mBits.I2C_enable = 0;
	PUT32(BSC1_I2C_CONTROL_REG,i2cctrlreg.mAsU32);	
}

int i2c_write(char device_addr, char* dataptr, int length)
{
	int index = 0;
	clear_status_register();
	clear_fifos();
	i2c_set_slave_addr(device_addr);
	i2c_set_data_len(length);
	
	start_write_transfer();
	
	while(length > 0)
	{
		if(check_for_i2c_error()!=0)
		{
			return -1;
		}
		wait_till_tx_fifo_can_accept_data();
		i2c_put_data_byte(dataptr[index]);
		index++;
		length--;
	}

	
	//Check if transfer done
	wait_till_transfer_done();
	
	i2c_stop_transfer();
	
	return 0;
}

int i2c_write_reg(char device_addr, char reg, char* dataptr, int length)
{
	int index = 0;
	clear_status_register();
	clear_fifos();
	i2c_set_slave_addr(device_addr);
	i2c_set_data_len(length+1); //+1 for the register address and data
	
	start_write_transfer();
	
	//Put the register address
	if(check_for_i2c_error()==0)
	{
		wait_till_tx_fifo_can_accept_data();
		i2c_put_data_byte(reg);
	}

	//Put the data
	while(length > 0)
	{
		if(check_for_i2c_error()!=0)
		{
			return -1;
		}
		wait_till_tx_fifo_can_accept_data();
		i2c_put_data_byte(dataptr[index]);
		index++;
		length--;
	}

	//Check if transfer done
	wait_till_transfer_done();
	
	i2c_stop_transfer();
	return 0;
}

void wait_till_tx_fifo_can_accept_data(void)
{
	volatile BSC1_I2C_status_Reg_t i2cstatusreg;
	while(1)
	{
		i2cstatusreg.mAsU32 = GET32(BSC1_I2C_STATUS_REG);
		if(i2cstatusreg.mBits.txd_fifo_can_accept_data == 1)
		{
			break;
		}
	}
}

void wait_till_transfer_done(void)
{
	volatile BSC1_I2C_status_Reg_t i2cstatusreg;
	while(1)
	{
		i2cstatusreg.mAsU32 = GET32(BSC1_I2C_STATUS_REG);
		if(i2cstatusreg.mBits.transfer_done == 1)
		{
			break;
		}
	}
}
	
int wait_till_rx_fifo_contains_data(int timeout)
{
	volatile BSC1_I2C_status_Reg_t i2cstatusreg;
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	while(1)
	{
		i2cstatusreg.mAsU32 = GET32(BSC1_I2C_STATUS_REG);
		if(i2cstatusreg.mBits.rxd_fifo_contains_data == 1)
		{
			break;
		}
		if((GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_tick) > (timeout*1000))
		{
			return -1;
		}
	}
	return 0;
}

int i2c_read(char device_addr, char* dataptr, int length)
{
	int count;
	clear_status_register();
	clear_fifos();
	i2c_set_slave_addr(device_addr);
	i2c_set_data_len(length);
	start_read_transfer();
	count = 0;
	volatile char temp_reg= 0x00;
	
	while(count < length)
	{
		if(check_for_i2c_error()!=0)
		{
			return -1;
		}
		if((wait_till_rx_fifo_contains_data(I2C_TIMEOUT_MS)) == -1)
		{return -1;}
		i2c_get_data_byte(&temp_reg);
		dataptr[count] = temp_reg;
		count++;
	}
	
	i2c_stop_transfer();
		
	return 0;
}

int i2c_read_reg(char device_addr, char reg,char* dataptr, int length)
{
	i2c_write(device_addr,&reg,1);
	i2c_read(device_addr,dataptr,length);
	return 0;
}