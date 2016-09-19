#include "./../include/utility.h"
#include "./../include/Bluefruit.h"
#include "./../include/SPI.h"
#include "./../include/MiniUART.h"


#define BLUEFRUIT_SPI_CHIP_SELECT 1


int bluefruit_init()
{
	send_at_command("AT\r");
	return 0;
}

int get_at_cmd_response()
{
	char response[128];
	spi_getbytes(BLUEFRUIT_SPI_CHIP_SELECT,(unsigned char*)response,128);
	if(strcmp(response,"OK\r\n") == 0)
	{
		uart_print_string_newline("Bluefruit OK response");
		return 0;
	}
	else
	{
		uart_print_string_newline("Bluefruit Error response");
		return -1;
	}
}

int send_at_command(char* at_cmd)
{
	spi_sendbytes(BLUEFRUIT_SPI_CHIP_SELECT,strlen(at_cmd),(unsigned char*)at_cmd);
	return 0;
}