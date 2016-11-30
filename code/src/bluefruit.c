#include "./../include/utility.h"
#include "./../include/bluefruit.h"
#include "./../include/SPI.h"
#include "./../include/MiniUART.h"

const char* bluefruit_at_commands[] = {
	"AT",
	"ATI",
	"ATZ",
	"AT+HELP",
	//General purpose
	"AT+FACTORYRESET",
	"AT+NVMWRITE",
	"AT+NVMREAD"
	//hardware
	"AT+BAUDRATE",
	"AT+HWADC",
	"AT+HWGETDIETEMP",
	"AT+HWGPIO",
	"AT+HWGPIOMODE",
	"AT+HWI2CSCAN",
	"AT+HWVBAT",
	"AT+HWRANDOM",
	"AT+HWMODELED",
	"AT+UARTFLOW",
	//BLE generic
	"AT+BLEPOWERLEVEL",
	"AT+BLEGETADDRTYPE",
	"AT+BLEGETADDR",
	"AT+BLEGETPEERADDR",
	"AT+BLEGETRSSI",
	//BLE services
	"AT+BLEUARTTX",
	"AT+BLEUARTX",
	"AT+BLEUARTFIFO",
	"AT+BLEKEYBOARDEN",
	"AT+BLEKEYBOARD",
	"AT+BLEKEYBOARDCODE",
	"AT+BLEHIDEN",
	"AT+BLEHIDMOUSEMOVE",
	"AT+BLEHIDMOUSEBUTTON",
	"AT+BLEHIDCONTROLKEY",
	"AT+BLEHIDGAMEPAD",
	"AT+BLEMIDIEN",
	"AT+BLEMIDIRX",
	"AT+BLEMIDITX",
	"AT+BLEBATTEN",
	"AT+BLEBATTVAL",
};

int bluefruit_init()
{
	uart_printf("\nInitializing bluefruit...");
	send_at_command("AT");
	send_at_command("AT");
	send_uart_message("Ready to go");
	send_uart_message(" Raspberry pi 2");
	return 0;
}

//Return 
/*
0 - status ok
-1 - Status Error
-2 - Status other
-3 - slave not ready
-4 - slave overflow
*/
int get_at_cmd_response(int command_type)
{
	int i =0,status = -3;;
	unsigned int msg_length = 0;
	char response[20];
	memset(response,0x00,20);
	uart_printf("\nReading AT response\n");
	while((status == -3) || (status == -4))
	{
		memset(response,0x00,20);
		spi_getbytes(BLUEFRUIT_SPI_CHIP_SELECT,(unsigned char*)response,20);
		i=0;
		while(1)
		{	
			if((response[i] == BLUEFRUIT_SLAVE_READ_OVERFLOW) || (i == 20))
			{
				break;
			}
			uart_printf("%x ",response[i]);
			i++;
		}
		//First byte of packet is SDEP response
		if(response[0] == SDEP_RESPONSE)
		{
			//Check for comamnd type
			if((response[1] == (char)(command_type&0xFF)) && (response[2] == (char)((command_type>>8)&0xFF)))
			{
				msg_length = (unsigned int)response[3];
				if(msg_length>0)
				{
					if(strncmp(&response[4],"OK\r\n",msg_length) == 1)
					{
						uart_printf("\nBluefruit OK response");
						status = 0;
						break;
					}
					else if(strncmp(&response[4],"Error\r\n",msg_length) == 1)
					{
						uart_printf("\nBluefruit Error response");
						status =  -1;
						break;
					}
				}
				else
				{
					uart_printf("\nBluefruit zero OK response");
					status = 0;
					break;
				}
			}
			else
			{
				uart_printf("\nBluefruit error: Command type mismatch error");
				status = -2;
				break;
			}
		}
		else if(response[0] == SDEP_ERROR)
		{
			uart_printf("\nBluefruit error: Command return SDEP_ERROR");
			status = -2;
			break;
		}
		else if(response[0] == SDEP_ALERT)
		{
			uart_printf("\nBluefruit error: Received Alert");
			status = -2;
			break;
		}
		else if(response[0] == BLUEFRUIT_SLAVE_NOT_READY)
		{
			uart_printf("\nBluefruit error: Slave not ready");
			time_sleep_us(100);
			status = -3;
		}
		else if(response[0] == BLUEFRUIT_SLAVE_READ_OVERFLOW)
		{
			uart_printf("\nBluefruit error: Slave data overflow");
			time_sleep_us(100);
			status = -4;
		}
	}
	return status;
}

int send_at_command(char* at_cmd)
{
	int i =0;
	unsigned char cmd[20];
	//send at command
	uart_printf("\nsending at command %s",at_cmd);
	memset(cmd,0x00,20);
	cmd[0] = SDEP_COMMAND;
	cmd[1] = (SDEP_CMDTYPE_AT_WRAPPER & 0xFF);
	cmd[2] = ((SDEP_CMDTYPE_AT_WRAPPER >> 8) & 0xFF);
	cmd[3] = strlen(at_cmd);
	for(i =0; i< strlen(at_cmd); i++)
	{
		cmd[4+i] = at_cmd[i];
	}

	spi_sendbytes(BLUEFRUIT_SPI_CHIP_SELECT,(SDEP_COMMAND_HEADER_SIZE+strlen(at_cmd)),cmd);
	//time_sleep(5);
	return(get_at_cmd_response(SDEP_CMDTYPE_AT_WRAPPER));
}

int send_uart_message(char* msg)
{
	int i =0;
	unsigned char cmd[20];
	//send at command
	uart_printf("\nsending uart message");
	memset(cmd,0x00,20);
	memset(cmd,0x00,20);
	cmd[0] = SDEP_COMMAND;
	cmd[1] = (SDEP_CMDTYPE_BLE_UARTTX & 0xFF);
	cmd[2] = ((SDEP_CMDTYPE_BLE_UARTTX >> 8) & 0xFF);
	cmd[3] = strlen(msg);
	for(i =0; i< strlen(msg); i++)
	{
		cmd[4+i] = msg[i];
	}
	spi_sendbytes(BLUEFRUIT_SPI_CHIP_SELECT,(SDEP_COMMAND_HEADER_SIZE+strlen(msg)),cmd);
	//time_sleep(5);
	return(get_at_cmd_response(SDEP_CMDTYPE_BLE_UARTTX));

}