#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/SystemTimer.h"
#include "./../include/interrupts.h"
#include "./../include/utility.h"
#include "./../include/synchronization.h"

#include <stdarg.h>

#define MAX_DIGITS_IN_32_BIT_NUMBER 10
unsigned char uart_buf[UART_BUF_SIZE];


int uart_mutex = 1;

unsigned char calc_checksum(unsigned char* ptr,int number_of_bytes)
{
	unsigned char sum = 0;
	int i =0;
	for(i = 0; i <number_of_bytes ; i++)
	{
		sum+= ptr[i];
	}
	return sum;
}

void init_uart_buf(void)
{
	int i =0;
	for(i=0;i<UART_BUF_SIZE;i++)
	{
		uart_buf[i] = '0';
	}
}

void uart_print_string(char* str)
{
	while(*str != '\0')
	{
		uart_putchar(*str);
		str++;
	}
	wait_till_transmitter_done();

}

void uart_print_string_newline(char* str)
{
	uart_putchar(0x0D);
	uart_putchar(0x0A);
	while(*str != '\0')
	{
		uart_putchar(*str);
		str++;
	}	
	wait_till_transmitter_done();
}

void uart_printf(char* print_str,...)
{
	//Enter UART critical section
	enter_critical_section(&uart_mutex);
	
    va_list arguments;   
	va_start(arguments,print_str);
	int print_seq_start = 0;
	
	while(*print_str != '\0')
	{
		switch(*print_str)
		{
			case '%':
				print_str++;
				print_seq_start = 1;
				break;
			case 'u':
				if(print_seq_start == 1)
				{
					uart_print_unsigned_number(va_arg(arguments,unsigned int));
					print_seq_start = 0;
				}
				else
				{
					uart_putchar(*print_str);
				}
				print_str++;
				break;
			case 'd':
				if(print_seq_start == 1)
				{
					uart_print_number(va_arg(arguments,int));
					print_seq_start = 0;
				}
				else
				{
					uart_putchar(*print_str);
				}
				print_str++;
				break;
			case 'c':
				if(print_seq_start == 1)
				{
					uart_putchar(va_arg(arguments,unsigned int));
					print_seq_start = 0;
				}
				else
				{
					uart_putchar(*print_str);
				}
				print_str++;
				break;
			case 's':
				if(print_seq_start == 1)
				{					
					uart_print_string(va_arg(arguments,char*));
					print_seq_start = 0;
				}
				else
				{
					uart_putchar(*print_str);
				}
				print_str++;
				break;
			case 'x':
				if(print_seq_start == 1)
				{
					uart_print_number_int_hex(va_arg(arguments,unsigned int));
					print_seq_start = 0;
				}
				else
				{
					uart_putchar(*print_str);
				}
				print_str++;
				break;
			case '\n':
				uart_putchar(0x0D);
				uart_putchar(0x0A);
				print_str++;
				break;
			case '\t':
				uart_print_string("    ");
				print_str++;
				break;
			case '\'':
				uart_putchar('\'');
				print_str++;
				break;
			case '"':
				uart_putchar('"');
				print_str++;
				break;
			default:
				uart_putchar(*print_str);
				print_str++;
				break;
		}
	}
		
	va_end(arguments);
	//Enter UART critical section
	exit_critical_section(&uart_mutex);

}

void uart_print_number_int_hex(unsigned int num)
{
	int index = 0;
	int i = 0;
	int remainder = 0;
	char num_to_char[10]= {'0','0','0','0','0','0','0','0','0','0'};
	uart_print_string("0x");
	if(num == 0)
	{
		uart_putchar('0');
	}
	else
	{
		while(num != 0)
		{
			remainder = num%16;
			switch (remainder)
			{
				case 0:
					num_to_char[index] = '0';
					break;
				case 1:
					num_to_char[index] = '1';
					break;
				case 2:
					num_to_char[index] = '2';
					break;
				case 3:
					num_to_char[index] = '3';
					break;
				case 4:
					num_to_char[index] = '4';
					break;
				case 5:
					num_to_char[index] = '5';
					break;					
				case 6:
					num_to_char[index] = '6';
					break;
				case 7:
					num_to_char[index] = '7';
					break;
				case 8:
					num_to_char[index] = '8';
					break;
				case 9:
					num_to_char[index] = '9';
					break;
				case 10:
					num_to_char[index] = 'A';
					break;
				case 11:
					num_to_char[index] = 'B';
					break;
				case 12:
					num_to_char[index] = 'C';
					break;
				case 13:
					num_to_char[index] = 'D';
					break;
				case 14:
					num_to_char[index] = 'E';
					break;
				case 15:
					num_to_char[index] = 'F';
					break;
				default:
					break;
			}
			num = (num/16);
			index++;
		}
		for(i =index-1; i >=0 ; i--)
		{
			uart_putchar(num_to_char[i]);
		}
	}
	wait_till_transmitter_done();
}

void uart_print_unsigned_number(unsigned int num)
{
	int index = 0;
	int i = 0;
	char num_to_char[MAX_DIGITS_IN_32_BIT_NUMBER]= {'0','0','0','0','0','0','0','0','0','0'};
	if(num == 0)
	{
		uart_putchar('0');
	}
	else
	{
		while(num != 0)
		{
			num_to_char[index] = (num%10) + '0';
			num = num/10;
			index++;
		}
		for(i =index-1; i >=0 ; i--)
		{
			uart_putchar(num_to_char[i]);
			if((i%3 == 0) && (i!=0))
			{
				uart_putchar(',');
			}
		}
	}
	wait_till_transmitter_done();
}

void uart_print_number(int num)
{
	int index = 0;
	int i = 0;
	char num_to_char[MAX_DIGITS_IN_32_BIT_NUMBER]= {'0','0','0','0','0','0','0','0','0','0'};
	if(num == 0)
	{
		uart_putchar('0');
	}
	else
	{
		if(num < 0)
		{
			uart_putchar('-');
			num = -num;
		}
		while(num != 0)
		{
			num_to_char[index] = (num%10) + '0';
			num = num/10;
			index++;
		}
		for(i =index-1; i >=0 ; i--)
		{
			uart_putchar(num_to_char[i]);
			if((i%3 == 0) && (i!=0))
			{
				uart_putchar(',');
			}
		}
	}
	wait_till_transmitter_done();
}

int uart_recvchar(unsigned char* data,unsigned int timeout_ms)
{
	volatile AUX_MU_STAT_Reg_t MUstatus;
	MUstatus.mAsU32 = 0;
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	
	AUX_MU_IO_Reg_t MUIoReg;	
	while(1)
	{
		MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
		if(MUstatus.mbits.symbol_available == 1)
		{
			break;
		}
		if((GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_tick) > (timeout_ms*1000))
		{
			return -1;
		}				
	}
	MUIoReg.mAsU32 = GET32(AUX_MU_IO_REG);
	*data = (char)(MUIoReg.receive_data.received_data);
	return 0;
}

void uart_putchar(unsigned char c)
{
	volatile AUX_MU_STAT_Reg_t MUstatus;
	volatile AUX_MU_IO_Reg_t MUIoReg;
	MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
	while(1)
	{
		MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
		if(MUstatus.mbits.transmit_space_available == 1)
		{
			break;
		}
	}
		
	MUIoReg.mAsU32 = GET32(AUX_MU_IO_REG);
	MUIoReg.transmit_data.transmit_data = c;
	PUT32(AUX_MU_IO_REG,MUIoReg.mAsU32);	
}

int wait_till_transmitter_done(void)
{
	volatile AUX_MU_STAT_Reg_t MUstatus;
	while(1)
	{
		MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
		if(MUstatus.mbits.transmitter_done == 1)
		{
			break;
		}	
	}
	return 0;
}

int UART_disable_transmitter_and_receiver(void)
{
	//Disable transmitter and receiver
	AUX_MU_CNTL_Reg_t cntrlreg;
	cntrlreg = (AUX_MU_CNTL_Reg_t)GET32(AUX_MU_CNTL_REG);
	cntrlreg.mbits.receiver_enable = 0;
	cntrlreg.mbits.transmitter_enable = 0;
	PUT32(AUX_MU_CNTL_REG,cntrlreg.mAsU32);
	return 0;
}

int UART_enable_transmitter_and_receiver(void)
{
	AUX_MU_CNTL_Reg_t cntrlreg;
	//Enable transmitter and receiver
	cntrlreg = (AUX_MU_CNTL_Reg_t)GET32(AUX_MU_CNTL_REG);
	cntrlreg.mbits.receiver_enable = 1;
	cntrlreg.mbits.transmitter_enable = 1;
	PUT32(AUX_MU_CNTL_REG,cntrlreg.mAsU32);
	return 0;
}

int UART_clear_FIFOs(void)
{
	//Clear the FIFOs
	AUX_MU_IIR_Reg_t IIRReg;
	IIRReg = (AUX_MU_IIR_Reg_t)GET32(AUX_MU_IIR_REG);
	IIRReg.mbits.Interrupt_ID = 3;
	IIRReg.mbits.fifo_enables = 3;
	PUT32(AUX_MU_IIR_REG,IIRReg.mAsU32);
	return 0;
}

int UARTInit(void)
{
	//Enable MiniUART
	AUX_Enable_Reg_t auxenablereg;
	auxenablereg = (AUX_Enable_Reg_t)GET32(AUX_ENABLE_REG);
	auxenablereg.mbits.mini_UART_enable = 1;
	PUT32(AUX_ENABLE_REG,auxenablereg.mAsU32);
	
	//Select Alternate function 5 for GPIO14 and 15
	GPIO_Funtion_Select1_t gpiofnsel1;
	gpiofnsel1 = (GPIO_Funtion_Select1_t)GET32(GPIO_REG_GPFSEL1);
	gpiofnsel1.mBits.FunctionSelGPIO14 = GPIO_SET_alternate_function_5;
	gpiofnsel1.mBits.FunctionSelGPIO15 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL1,gpiofnsel1.mAsU32);
	
	//Disable transmitter and receiver
	UART_disable_transmitter_and_receiver();
	
	//Clear Interrrupts
	AUX_MU_IER_Reg_t IERReg;
	IERReg = (AUX_MU_IER_Reg_t)GET32(AUX_MU_IER_REG);
	IERReg.Interrupt_bits.Enable_transmit_intr = 0;
	IERReg.Interrupt_bits.Enable_receive_intr  = 0;
	PUT32(AUX_MU_IER_REG,IERReg.mAsU32);
	
	//Clear the FIFOs
	UART_clear_FIFOs();
	
	//Set the data size to 8 bit
	AUX_MU_LCR_Reg_t LCRReg;
	LCRReg = (AUX_MU_LCR_Reg_t)GET32(AUX_MU_LCR_REG);
	LCRReg.mbits.data_size = 3;
	LCRReg.mbits.dlab_acess = 0;
	PUT32(AUX_MU_LCR_REG,LCRReg.mAsU32);
	
	//Set the baud rate to 115200
	AUX_MU_BAUD_RATE_Reg_t baudreg;
	baudreg = (AUX_MU_BAUD_RATE_Reg_t)GET32(AUX_MU_BAUD_REGISTER);
	baudreg.mbits.baud_rate = 270; //115200 baud rate
	PUT32(AUX_MU_BAUD_REGISTER,baudreg.mAsU32);
	
	//Enable transmitter and receiver
	UART_enable_transmitter_and_receiver();
	
	return 0;
}

int xmodem_send(void*ptr , int byte_count)
{
	unsigned char temp= '0';
	//int i =0;
	while(1)
	{
		if(uart_recvchar(&temp,1000) == 0)
		{
			if(temp == XMODEM_NAK)
			{
				break;
			}
		}		
	}
		
	return 0;
}

void flush_uart_rx_buffer(void) {
    unsigned char c;
    while (uart_recvchar(&c, 100) == 0);
}


int xmodem_recv(void* recv_ptr)
{
	unsigned int char_index = 1;
	unsigned char seq_num = 1;
	unsigned int byte_count = 0;
	int SOH_received = -1;

	LEDTurnon();


	//Wait till we receive a character
	while(1)
	{		
		uart_recvchar(&uart_buf[0],10000);
		
		if(uart_buf[0] == XMODEM_SOH)
		{
			SOH_received = 1;
			while(char_index < UART_BUF_SIZE)
			{
				if(uart_recvchar(&uart_buf[char_index],1000) == 0)
				{
					char_index++;
				}
				else
				{
					break;
				}
			}
			//At this point we have the whole packet or we dont
			if((uart_buf[1] ==seq_num) && (uart_buf[2] == (unsigned char)(~seq_num)) && (uart_buf[131] == (calc_checksum(&uart_buf[3],XMODEM_PAYLOAD_SIZE))))
			{
				SOH_received = 0;
				seq_num++;
				char_index = 1;				
				memcpy((recv_ptr+byte_count),&uart_buf[3],XMODEM_PAYLOAD_SIZE);
				byte_count += XMODEM_PAYLOAD_SIZE;
				init_uart_buf();
				uart_putchar(XMODEM_ACK);
			}
			else
			{
				SOH_received = 0;
				char_index = 1;
				init_uart_buf();
				flush_uart_rx_buffer();
				uart_putchar(XMODEM_NAK);
			}
		}
		else if(uart_buf[0] == XMODEM_EOT)
		{
			LEDTurnoff();
			uart_putchar(XMODEM_ACK);
			SOH_received = -1;
			return byte_count;
		}
		else if(uart_buf[0] == XMODEM_CAN)
		{
			char_index = 1;
			seq_num = 1;
			byte_count = 0;
			uart_putchar(XMODEM_ACK);
			SOH_received = -1;
		}
		else if(SOH_received == -1)
		{
			uart_putchar(XMODEM_NAK);
		}
	}
	
	return 0;
	
}
int uart_tests(void)
{
	uart_printf("\nABCDEFGHIJKLMNOPQRSTUVWXYZ");
	uart_printf("\nabcdefghijklmnopqrstuvwxyz");
	uart_printf("\n0123456789");
	uart_printf("\n9876543210");
	return 0;
}

void uart_printf_tests(void)
{
	uart_printf("\nsimple string\n");
	uart_printf("New tab\t new line\n");
	uart_printf("char : %c\n",'a');
	uart_printf("char : %c\n",'9');
	uart_printf("char : %c\n",'A');
	uart_printf("char : %c\t , char2: %c\t , char3: %c\t, string: %s\n",'A','B','C',"abcdef");
	uart_printf("String : %s\n","abcdefghi");
	uart_printf("Number : %d\n", 100);
	uart_printf("Hex of number %d is : %x\n",100,100);
	uart_printf("Number : %d\n", -200);
	uart_printf("Number : %d\n", 1700);
	uart_printf("Number : %d\n", 123456789);
	uart_printf("unsigned number : %u\n", 123456789);
	uart_printf("unsigned number : %u\n", -100);
	uart_printf("char : %c\t , Number: %d\t , hex: %x\t, string: %s\n",'A',5000,5000,"abcdef");

}
