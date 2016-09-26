#include "./../include/I2C.h"
#include "./../include/Ft6206_cap_touch.h"
#include "./../include/utility.h"
#include "./../include/MiniUART.h"

short touch_x0= 0;
short touch_y0= 0;
char  pressure_0 = 0x00;
short touch_x1= 0;
short touch_y1= 0;
char  pressure_1 = 0x00;

int FT6206_get_id(char* id)
{
	char reg = FT6206_REG_VENDID;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,id,1);
	if(*id == FOCAL_TECH_ID){
		uart_print_string_newline("FT6206 Capacitive touch device found:");	
		return 0;
	}
	else{
		uart_print_string_newline("FT6206 Capacitive touch device not found:");		
		*id = 0x00;
		return -1;
	}
		
}


int FT6206_get_num_touches(void)
{
	short x =0; short y =0;char pressure = 0;
	char reg = FT6206_TD_STATUS;
	TD_status_reg_t num_touches;
	memset((char*)&(num_touches.mAsU8),0x00,sizeof(TD_status_reg_t));
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&(num_touches.mAsU8),1);
	if(num_touches.mBits.number_of_touch_points == 1)
	{
		uart_print_string_newline("Get Num touches:");
		uart_print_number((int)(num_touches.mBits.number_of_touch_points));
		FT6206_get_touch_point_1(&x,&y,&pressure);
		uart_print_string_newline("Touchpoint1:");
		uart_print_number((int)x);
		uart_putchar(',');
		uart_print_number((int)y);
		uart_putchar(',');
		uart_print_number((int)pressure);
	}
	else if(num_touches.mBits.number_of_touch_points == 2)
	{
		uart_print_string_newline("Get Num touches:");
		uart_print_number((int)(num_touches.mBits.number_of_touch_points));
		FT6206_get_touch_point_1(&x,&y,&pressure);
		uart_print_string_newline("Touchpoint1:");
		uart_print_number((int)x);
		uart_putchar(',');
		uart_print_number((int)y);
		uart_putchar(',');
		uart_print_number((int)pressure);
		FT6206_get_touch_point_2(&x,&y,&pressure);
		uart_print_string_newline("Touchpoint2:");
		uart_print_number((int)x);
		uart_putchar(',');
		uart_print_number((int)y);
		uart_putchar(',');
		uart_print_number((int)pressure);
	}

	return (int)(num_touches.mBits.number_of_touch_points);
}

int FT6206_get_chip_select(void)
{
	char reg = FT6206_REG_CHIP_SELECT;
	uart_print_string_newline("Get Chip select:");
	char chip_select = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&chip_select,1);
	uart_print_number((int)chip_select);
	return (int)(chip_select);
}

int FT6206_get_period_active_mode(void)
{
	char reg = FT6206_REG_PERIOD_ACTIVE;
	uart_print_string_newline("Get period active mode:");
	char period = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&period,1);
	uart_print_number((int)period);
	return (int)(period);
}

int FT6206_get_gesture_id(void)
{
	char reg = FT6206_GESTURE_ID;
	uart_print_string_newline("Gesture ID:");
	char gesture_id = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&gesture_id,1);
	uart_print_number((int)gesture_id);
	return (int)(gesture_id);
}

int FT6206_get_control(void)
{
	char reg = FT6206_REG_CONTROL;
	uart_print_string_newline("Get Control:");
	char ctrl = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&ctrl,1);
	uart_print_number((int)ctrl);
	return (int)(ctrl);
}

int FT6206_get_power_mode(void)
{
	char reg = FT6206_REG_PWRMODE;
	uart_print_string_newline("Get power mode:");
	char pwr_mode = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&pwr_mode,1);
	uart_print_number((int)pwr_mode);
	return (int)(pwr_mode);
}

int FT6206_get_interrupt_mode(void)
{
	char reg = FT6206_REG_INTERRUPT_MODE;
	uart_print_string_newline("Get interrupt mode:");
	char interrupt_mode = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&interrupt_mode,1);
	uart_print_number((int)interrupt_mode);
	return (int)(interrupt_mode);
}

int FT6206_set_interrupt_mode(char mode)
{
	char reg = FT6206_REG_INTERRUPT_MODE;
	uart_print_string_newline("Set interrupt mode:");
	char interrupt_mode = 0xFF;
	
	// read first 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&interrupt_mode,1);
	time_sleep(300);
	
	//write
	i2c_write_reg(FT6206_SLAVE_ADDR,reg,(char*)&mode,1);
	time_sleep(300);
	
	// read second 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&interrupt_mode,1);
	uart_print_number((int)(interrupt_mode));
	time_sleep(300);
	
	return 0;
}


int FT6206_get_current_operating_state(void)
{
	char reg = FT6206_REG_STATE;
	uart_print_string_newline("Get State:");
	char state = 0xFF;
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&state,1);
	uart_print_number((int)state);
	return (int)(state);
}

int FT6206_set_control(char mode)
{
	char reg = FT6206_REG_CONTROL;
	uart_print_string_newline("Set Control:");
	char ctrl_mode = 0xFF;
	
	// read first 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&ctrl_mode,1);
	time_sleep(300);
	
	//write
	i2c_write_reg(FT6206_SLAVE_ADDR,reg,(char*)&mode,1);
	time_sleep(300);
	
	// read second 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&ctrl_mode,1);
	uart_print_number((int)(ctrl_mode));
	time_sleep(300);
	
	return 0;
}

int FT6206_get_device_mode(void)
{
	char reg = FT6206_REG_MODE;
	char device_mode= 0xFF;
	uart_print_string_newline("Get Device mode:");
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&device_mode,1);
	uart_print_number((int)(device_mode));
	return (int)(device_mode);
}

int FT6206_set_device_mode(char dev_mode)
{
	char reg = FT6206_REG_MODE;
	char device_mode= 0xFF;
	uart_print_string_newline("Set Device mode:");
	
	// read first 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&device_mode,1);
	time_sleep(300);
	
	i2c_write_reg(FT6206_SLAVE_ADDR,reg,(char*)&dev_mode,1);
	time_sleep(300);
	
	// read second 
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,(char*)&device_mode,1);
	uart_print_number((int)(device_mode));
	time_sleep(300);
	
	
	return (int)(device_mode);
}

int FT6206_set_threshold(char value)
{
	char reg = FT6206_REG_THRESHHOLD;
	char device_threshold= 0xFF;
	
	uart_print_string_newline("Device Threshold:");
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&device_threshold,1);
	time_sleep(300);
	uart_print_number((int)(device_threshold));
	device_threshold = 0xFF;
	
	i2c_write_reg(FT6206_SLAVE_ADDR,reg,&value,1);
	time_sleep(300);
	i2c_read_reg(FT6206_SLAVE_ADDR,reg,&device_threshold,1);
	time_sleep(300);

	uart_print_number((int)(device_threshold));
	return 0;
}

int FT6206_get_touch_point_1(short* x , short* y, char* pressure)
{
	Pn_XH_t P1_XH;
	Pn_YH_t P1_YH;
	char P1_XL = 0;
	char P1_YL = 0;
	P1_XH.mAsU8 = 0x00;
	P1_YH.mAsU8 = 0x00;
	
	char reg = FT6206_P1_XH;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,(char*)&P1_XH,1);
	
	reg = FT6206_P1_XL;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,&P1_XL,1);
	
	*x = (P1_XH.mBits.touch_X_position_11_8 << 8) | (P1_XL);
	
	reg = FT6206_P1_YH;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,(char*)&P1_YH,1);
	
	reg = FT6206_P1_YL;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,&P1_YL,1);
	
	*y = (P1_YH.mBits.touch_Y_position_11_8 << 8) | (P1_YL);
	
	reg = FT6206_P1_WEIGHT;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,pressure,1);
	
	return 0;
}

int FT6206_get_touch_point_2(short* x , short* y, char* pressure)
{
	Pn_XH_t P2_XH;
	Pn_YH_t P2_YH;
	char P2_XL;
	char P2_YL;
	
	char reg = FT6206_P2_XH;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,(char*)&P2_XH,1);
	
	reg = FT6206_P2_XL;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,&P2_XL,1);
	
	*x = (P2_XH.mBits.touch_X_position_11_8 << 11) | (P2_XL);
	
	reg = FT6206_P2_YH;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,(char*)&P2_YH,1);
	
	reg = FT6206_P2_YL;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,&P2_YL,1);
	
	*y = (P2_YH.mBits.touch_Y_position_11_8 << 11) | (P2_YL);
	
	reg = FT6206_P2_WEIGHT;
	i2c_write(FT6206_SLAVE_ADDR,&reg,1);
	i2c_read(FT6206_SLAVE_ADDR,pressure,1);

	return 0;
}

void FT6206_tests(void)
{
	char touchid[4];

	//I2c tests
	memset(touchid,0x00,4);
	FT6206_get_id(&touchid[0]);
	FT6206_get_chip_select();
	FT6206_get_period_active_mode();
	FT6206_get_chip_select();
	FT6206_set_device_mode(WORKING_MODE);
	FT6206_get_id(&touchid[1]);
	FT6206_get_power_mode();
	FT6206_get_device_mode();
	FT6206_get_current_operating_state();
	FT6206_set_threshold(0x80);
	FT6206_set_control(ACTIVE_MODE);
	FT6206_get_interrupt_mode();
	FT6206_set_interrupt_mode(0x00);
}