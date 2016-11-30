#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/SPI.h"
#include "./../include/display.h"
#include "./../Image/a-img.h"
//#include "./../Image/f-img.h"
//#include "./../Image/atoz-img.h"
#include "./../include/I2C.h"
#include "./../fonts/Arial15x15.h"
#include "./../fonts/Terminal_font_16.h"
#include "./../include/Ft6206_cap_touch.h"
#include "./../include/DMA.h"
#include "./../include/Interrupts.h"
#include "./../include/SystemTimer.h"


#define ILI9341_SPI_CHIP_SELECT 0


#if defined(BITS_PER_PIXEL_18)
unsigned char color_rgb_values[][BYTES_PER_PIXEL] = {
						{0x00,0x00,0x00}, 	//Black
						{0xFF,0xFF,0xFF},	//white
						{0xFF,0x00,0x00},	//Red
						{0x00,0xFF,0x00},	//Lime
						{0x00,0x00,0xFF},	//blue
						{0xFF,0xFF,0x00},	//yellow
						{0x00,0xFF,0xFF},	//aqua
						{0xFF,0x00,0xFF},	//pink
						{0xC0,0xC0,0xC0},	//Silver
						{0x80,0x80,0x80},	//gray
						{0x80,0x00,0x00},	//maroon
						{0x00,0x80,0x00},	//green
};
#elif defined(BITS_PER_PIXEL_16)
unsigned char color_rgb_values[][BYTES_PER_PIXEL] = {
						{0x00,0x00}, 	//Black
						{0xFF,0xFF},	//white
						{0xF8,0x00},	//Red
						{0x03,0xE0},	//Lime
						{0x00,0x1F},	//blue
						{0xFF,0xE0},	//yellow
						{0x07,0xFF},	//aqua
						{0xF8,0x1F},	//pink
						{0xC6,0x18},	//Silver
						{0x7B,0xEF},	//gray
						{0x78,0x00},	//maroon
						{0x07,0xE0},	//green
};
#endif


unsigned char test_id[4]={'0'};
unsigned int time_ms = 2000;

unsigned char TFT_Screen_frambuffer_test[512];
unsigned char TFT_Screen_frambuffer[TFT_SCREEN_BUFFER_SIZE_DMA];

//TFT screen and width
unsigned short TFT_Screen_width = 0;
unsigned short TFT_Screen_height = 0;

//Last written x and y co-ordinates in the screen
unsigned short dirty_x = 0;
unsigned short dirty_y = 0;

//Global params for Addr windows
unsigned short TFT_Screen_dirty_x0 = 0;
unsigned short TFT_Screen_dirty_y0 = 0;
unsigned short TFT_Screen_dirty_x1 = 0;
unsigned short TFT_Screen_dirty_y1 = 0;

//Font globals
#if defined(FONT_ARIAL_11_14)
	unsigned char* font_data = (unsigned char*)Arial11x14;
	unsigned int font_height = 14;
	unsigned int font_width = 11;
#elif defined(TERMINAL_12_16)
	unsigned char* font_data = (unsigned char*)Terminal12x16;
	unsigned int font_height = 16;
	unsigned int font_width = 12;
#endif

font_header_t font_header;


void get_font_header(void)
{
	memset((char*)&font_header,0x00,SIZE_OF_FONT_HEADER);
	font_header.font_id 		= (short)font_data[0];
	font_header.first_char_id 	= (short)((font_data[3] << 8) | (font_data[2]));
	font_header.last_char_id 	= (short)((font_data[5] << 8) | (font_data[4]));
	font_header.height 			= (short)(font_data[6]);
}

void init_frame_buffer(void)
{
	int i =0;
	for (i=0; i<TFT_DISPLAY_WIDTH*TFT_DISPLAY_HEIGHT*BYTES_PER_PIXEL; i++)
	{
		TFT_Screen_frambuffer[i] = 0x00;
	}
}

int ili9341_get_screen_memory(short x0, short y0, short x1, short y1)
{
	//int size = 0;
	ili9341_set_addr_window(x0,y0,x1,y1);
	//size = (((x1-x0)+1)*((y1-y0)+1)*BYTES_PER_PIXEL);
	unsigned char ptr[1];
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_READ,0,ptr);
	spi_getbytes(ILI9341_SPI_CHIP_SELECT,TFT_Screen_frambuffer_test, 512);
	return 0;
}

unsigned short ili9341_get_font_height()
{
	return font_height;
}

unsigned short ili9341_get_font_width()
{
	return font_width;
}


unsigned short ili9341_get_width()
{
	return TFT_Screen_width;
}

unsigned short ili9341_get_height()
{
	return TFT_Screen_height;
}

int ili9341_getid(unsigned char*ptr)
{
	//Does not work
	/*unsigned char data_ptr[1];
	data_ptr[0] = 0x11;
	spi_sendcommand(0xD9,1,data_ptr);
	spi_sendreadcommand(0xD3, 1, &id[0]);
	data_ptr[0] = 0x12;
	spi_sendcommand(0xD9,1,data_ptr);*/
	memset((char *)ptr,0x00,4);
	spi_sendreadcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_READ_DISPLAY_IDENTIFICATION_INFORMATION, 4, &ptr[0]);
	return 0;
}

void ili9341_turn_display_off(void)
{
	unsigned char ptr[1];
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_SLEEP_OUT,0,ptr);
	time_sleep(120);
}

void ili9341_init_tft_display()
{
	unsigned char ptr[10];
	memset((char*)ptr,0x00,10);
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_NOP,0,ptr);
	time_sleep(120);
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_SOFTWARE_RESET,0,ptr);
	time_sleep(120);
	
	/*ptr[0] = 0x03;
	ptr[1] = 0x80;
	ptr[2] = 0x02;
	spi_sendcommand(0xEF, 3, ptr);
		
	ptr[0] = 0x00;
	ptr[1] = 0xC1;
	ptr[2] = 0x30;
	spi_sendcommand(0xCF, 3,ptr);
	
	ptr[0] = 0x64;
	ptr[1] = 0x03;
	ptr[2] = 0x12;
	ptr[3] = 0x81;
	spi_sendcommand(0xED, 4,ptr);
	
	ptr[0] = 0x85;
	ptr[1] = 0x00;
	ptr[2] = 0x78;
	spi_sendcommand(0xE8, 3, ptr);
	
	ptr[0] = 0x39;
	ptr[1] = 0x2C;
	ptr[2] = 0x00;
	ptr[3] = 0x34;
	ptr[4] = 0x02;
	spi_sendcommand(0xCB, 5, ptr);
	
	ptr[0] = 0x20;
	spi_sendcommand(0xF7, 1, ptr);
	
	ptr[0] = 0x00;
	ptr[1] = 0x00;
	spi_sendcommand(0xEA, 2, ptr);

	// Power Control 1 
	ptr[0] = 0x23;
	spi_sendcommand(0xC0, 1, ptr);

	// Power Control 2
	ptr[0] = 0x10;
	spi_sendcommand(0xC1, 1, ptr);

	// VCOM Control 1
	ptr[0] = 0x3E;
	ptr[1] = 0x28;
	spi_sendcommand(0xC5, 2, ptr);

	// VCOM Control 2
	ptr[0] = 0x86;
	spi_sendcommand(0xC7, 1, ptr);

	// Frame Rate Control
	// Division ratio = fosc, Frame Rate = 79Hz
	ptr[0] = 0x00;
	ptr[1] = 0x18;
	spi_sendcommand(0xB1, 2, ptr);

	// Display Function Control
	ptr[0] = 0x08;
	ptr[1] = 0x82;
	ptr[2] = 0x27;
	spi_sendcommand(0xB6, 3, ptr);*/

	// Gamma Function Disable
	//spi_sendcommand(0xF2, 1, 0x00);

	// Gamma curve selected
	//spi_sendcommand(0x26, 1, 0x01);

	// Positive Gamma Correction
	//spi_sendcommand(0xE0, 15,
	//		0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
	//		0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);

	// Negative Gamma Correction
	//spi_sendcommand(0xE1, 15,
	//		0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
	//		0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);
	
	// COLMOD: Pixel Format Set
	// 16 bits/pixel
	ptr[0] = PIXEL_FORMAT_SET;
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_PIXEL_FORMAT_SET, 1, ptr);
		
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_SLEEP_OUT,0,ptr);
	time_sleep(120);

	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_DISPLAY_ON,0,ptr);
	time_sleep(120);
	
	init_frame_buffer();
}

int ili9341_set_screen_rotation(unsigned char rotation, int bgr)
{
	ILI9341_Mem_Access_Control_Reg_t rotation_param;
	rotation_param.mAsU8 = 0;
	
	if(rotation == 0)
	{
		if(bgr == 1)
		{
			rotation_param.mBits.BGR = 1;
		}
		rotation_param.mBits.MX_Column_address_order = 1;
		spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_ACCESS_CONTROL, 1, &(rotation_param.mAsU8));
		TFT_Screen_width = TFT_DISPLAY_WIDTH;
		TFT_Screen_height = TFT_DISPLAY_HEIGHT;		
		return 0;
	}
	else if(rotation == 1)
	{
		if(bgr == 1)
		{
			rotation_param.mBits.BGR = 1;
		}
		rotation_param.mBits.MV_Row_Column_Exchange = 1;
		spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_ACCESS_CONTROL, 1, &(rotation_param.mAsU8));
		TFT_Screen_width = TFT_DISPLAY_HEIGHT;
		TFT_Screen_height = TFT_DISPLAY_WIDTH;
		return 0;
	}
	else if(rotation == 2)
	{
		if(bgr == 1)
		{
			rotation_param.mBits.BGR = 1;
		}
		rotation_param.mBits.MY_Row_address_order = 1;
		spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_ACCESS_CONTROL, 1, &(rotation_param.mAsU8));
		TFT_Screen_width = TFT_DISPLAY_WIDTH;
		TFT_Screen_height = TFT_DISPLAY_HEIGHT;
		return 0;
	}
	else if(rotation == 3)
	{
		if(bgr == 1)
		{
			rotation_param.mBits.BGR = 1;
		}
		rotation_param.mBits.MX_Column_address_order = 1;
		rotation_param.mBits.MY_Row_address_order = 1;
		rotation_param.mBits.MV_Row_Column_Exchange = 1;
		spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_ACCESS_CONTROL, 1, &(rotation_param.mAsU8));
		TFT_Screen_width = TFT_DISPLAY_HEIGHT;
		TFT_Screen_height = TFT_DISPLAY_WIDTH;
		return 0;
	}
	else
	{
		return -1;
	}
}

int ili9341_update_display(void)
{
	int size =0;
	int i =0;	
	int test = 10;
	//size = ((TFT_Screen_dirty_x1-TFT_Screen_dirty_x0)+1) * ((TFT_Screen_dirty_y1-TFT_Screen_dirty_y0)+1);
	ili9341_set_addr_window(0,0,test,test);
	size = test*test;
	for(i=0;i<size;i++)
	{
		memcpy(&TFT_Screen_frambuffer[i*BYTES_PER_PIXEL], &color_rgb_values[COLOR_RED][0],BYTES_PER_PIXEL);
		spi_sendbytes(ILI9341_SPI_CHIP_SELECT,BYTES_PER_PIXEL, &TFT_Screen_frambuffer[i*BYTES_PER_PIXEL]);
	}
	
	return 0;

}

int ili9341_load_image(const unsigned char* img, int image_width, int image_height, int image_bytes_per_pixel)
{
	int size = 0;
	int i =0;
	unsigned char rgb[BYTES_PER_PIXEL];
	if((image_width > TFT_Screen_width) || (image_height > TFT_Screen_height) || (image_bytes_per_pixel != BYTES_PER_PIXEL))
	{
		return -1;
	}
	ili9341_set_addr_window(0,0,image_width,image_height);
	size = image_width*image_height;

	for(i = 0; i < size ; i++)
	{
		if(image_bytes_per_pixel == BYTES_PER_PIXEL)
		{
			rgb[0] = img[(i*image_bytes_per_pixel)+0];
			rgb[1] = img[(i*image_bytes_per_pixel)+1];
			spi_sendbytes(ILI9341_SPI_CHIP_SELECT,BYTES_PER_PIXEL, rgb);
		}
		else if(image_bytes_per_pixel == BYTES_PER_PIXEL)
		{
			rgb[2] = img[(i*image_bytes_per_pixel)+0];
			rgb[1] = img[(i*image_bytes_per_pixel)+1];
			rgb[0] = img[(i*image_bytes_per_pixel)+2];
			spi_sendbytes(ILI9341_SPI_CHIP_SELECT,BYTES_PER_PIXEL, rgb);
		}
	}
	
	return 0;
}

int ili9341_set_vertical_scrolling_definition(short top_fixed_area, short scroll_area, short bottom_fixed_area)
{
	unsigned char cmd_data[6];
	memset((char *)cmd_data,0x00,6);
	
	cmd_data[0] = (top_fixed_area >> 8) & 0xFF;
	cmd_data[1] = (top_fixed_area & 0xFF);
	cmd_data[2] = (scroll_area >> 8) & 0xFF;
	cmd_data[3] = (scroll_area & 0xFF);
	cmd_data[4] = (bottom_fixed_area >> 8) & 0xFF;
	cmd_data[5] = (bottom_fixed_area & 0xFF);
		
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_VERTICAL_SCROLL_DEFINITION,6,(unsigned char*)cmd_data);

	return 0;
}

int ili9341_set_vertical_scrolling_set_start_addr(short start_line)
{
	unsigned char cmd_data[2];
	memset((char *)cmd_data,0x00,2);
	
	cmd_data[0] = (start_line >> 8) & 0xFF;
	cmd_data[1] = (start_line & 0xFF);
		
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_VERTICAL_SCROLL_START_ADDR,2,(unsigned char*)cmd_data);

	return 0;
}

int ili9341_scroll(void)
{
	unsigned char cmd_data[1];
	memset((char *)cmd_data,0x00,1);
		
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_VERTICAL_SCROLL_START_ADDR,1,(unsigned char*)cmd_data);

	return 0;
}




int ili9341_set_addr_window(short x0, short y0, short x1, short y1)
{
	unsigned char cmd_data[4];
	memset((char *)cmd_data,0x00,4);
	cmd_data[0] =  x0 >> 8;
	cmd_data[1] =  x0 & 0xFF;
	cmd_data[2] =  x1 >> 8;
	cmd_data[3] =  x1 & 0xFF ;
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_COLUMN_ADDRESS_SET,4,(unsigned char*)cmd_data);
	
	memset((char*)cmd_data,0x00,4);
	cmd_data[0] =  y0 >> 8;
	cmd_data[1] =  y0 & 0xFF;
	cmd_data[2] =  y1 >> 8;
	cmd_data[3] =  y1 & 0xFF;
	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_PAGE_ADDRESS_SET,4,(unsigned char*)cmd_data);

	spi_sendcommand(ILI9341_SPI_CHIP_SELECT,ILI9341_CMD_MEMORY_WRITE,0,(unsigned char*)cmd_data);
	
	TFT_Screen_dirty_x0 = x0;
	TFT_Screen_dirty_y0 = y0;
	TFT_Screen_dirty_x1 = x1;
	TFT_Screen_dirty_y1 = y1;

	return 0;
}	

/*int ili9341_print_number(unsigned int number,int color)
{
}*/

int ili9341_print_string(char* print_string,int color)
{
	while(*print_string != '\0')
	{
		if((dirty_x+ili9341_get_font_width()) >= ili9341_get_width())
		{
			dirty_y+= ili9341_get_font_height();
			dirty_x = 0;
		}
		ili9341_print_char(*print_string,dirty_x,dirty_y,color);
		dirty_x += ili9341_get_font_width();
		print_string++;
	}
	return 0;
}

int scroll_mode = 0;
short scroll_height = 0; 
int ili9341_print_string_newline(char* print_string,int color)
{
	//Move to the next line
	if(dirty_x != 0)
	{
		dirty_y+= ili9341_get_font_height();
		dirty_x = 0;
		if(dirty_y >= ili9341_get_height())
		{
			//send scroll command
			scroll_mode = 1;
			dirty_y = 0;
		}
		if(scroll_mode == 1)
		{
			if(scroll_height >= ili9341_get_height())
			{
				scroll_height = 0;
			}
			scroll_height += ili9341_get_font_height();
			ili9341_set_vertical_scrolling_set_start_addr(scroll_height);
		}
	}

	while(*print_string != '\0')
	{
		if((dirty_x+ili9341_get_font_width()) >= ili9341_get_width())
		{
			//Move to new line
			dirty_y+= ili9341_get_font_height();
			dirty_x = 0;
			//Check if line goes out of bounds then set scroll  mode once
			if(dirty_y >= ili9341_get_height())
			{
				//send scroll command
				scroll_mode = 1;
				dirty_y = 0;
			}
			if(scroll_mode == 1)
			{
				if(scroll_height >= ili9341_get_height())
				{
					scroll_height = 0;
				}
				scroll_height += ili9341_get_font_height();
				ili9341_set_vertical_scrolling_set_start_addr(scroll_height);
			}
		}
		ili9341_print_char(*print_string,dirty_x,dirty_y,color);
		dirty_x += ili9341_get_font_width();
		print_string++;
	}
	//make the rest of the line bg color
	ili9341_fill_color(dirty_x,dirty_y,(ili9341_get_width()-1),(dirty_y+ili9341_get_font_height()), COLOR_BLACK);
	return 0;
}



int ili9341_print_char(char print_char,int x, int y,int color)
{
	char bitmap[28];
	character_table_t char_table;
	int char_index = 0;
	int i =0,j=0,k=0;
	int char_bitmap_numberofbytes_per_line = 0;
	int bitmap_index = -1;
	char bitmask = 0x01;
	
	unsigned char char_frame_buffer[font_height][font_width][BYTES_PER_PIXEL];
	unsigned int char_bitmap_offset = 0;

	
	//Initialize all arrays
	memset((void*)&char_table,0x00,sizeof(character_table_t));
	memset(char_frame_buffer, 0x00, font_height*font_width*BYTES_PER_PIXEL);
	memset(bitmap,0x00,28);
	
	
	if(((short)print_char < font_header.first_char_id) || ((short)print_char > font_header.last_char_id))
	{
		return -1;
	}
	
	char_index = (short)print_char - font_header.first_char_id;
	memcpy((void*)&char_table,(void*)&font_data[SIZE_OF_FONT_HEADER + (char_index * SIZE_OF_CHAR_TABLE)],SIZE_OF_CHAR_TABLE);
	
	char_bitmap_offset = ((int)(char_table.offset_23_16 << 16) | (int)(char_table.offset_15_8 << 8) | char_table.offset_7_0);
	
	char_bitmap_numberofbytes_per_line = calculate_number_of_bytes_per_line(char_table.width);
	
	memcpy((void*)bitmap, (void*)&font_data[char_bitmap_offset], char_bitmap_numberofbytes_per_line*font_header.height);
	
	for(i = 0; i < (int)(font_header.height); i++)
	{
		bitmap_index++;
		for(j = 0; j < (int)(char_table.width); j++)
		{
			if((j%8 == 0) && (j!=0))
			{
				bitmap_index++;
			}
			bitmask = (0x01 << (j%8));
			if((bitmask & bitmap[bitmap_index]) == bitmask)
			{	
				for(k = 0; k< BYTES_PER_PIXEL; k++)
				{
					char_frame_buffer[i][j][k] = color_rgb_values[color][BYTES_PER_PIXEL-1-k];
				}		
			}
		}
	}
	
	ili9341_set_addr_window(x,y,x+font_width-1,y+font_height-1);
	
	for(i =0; i < font_height; i++)
	{
		for(j = 0; j< font_width; j++)
		{
			spi_sendbytes(ILI9341_SPI_CHIP_SELECT,BYTES_PER_PIXEL,(unsigned char*)&char_frame_buffer[i][j][0]);
		}
	}
	
	return 0;
}

int calculate_number_of_bytes_per_line(int width)
{
	int number_of_bytes = 0;
	number_of_bytes = width/8;
	width = width%8;
	if(width > 0)
	{
		number_of_bytes++;
	}
	return number_of_bytes;
}

void ili9341_fill_color(short x0, short y0, short x1, short y1,int color)
{
	int size = 0;
	ili9341_set_addr_window(x0,y0,x1,y1);
	
	size = (((x1-x0)+1)*((y1-y0)+1)*BYTES_PER_PIXEL);
	//memfill_pattern(TFT_Screen_frambuffer,(char*)color_rgb_values[color] ,size,BYTES_PER_PIXEL);
	memset(TFT_Screen_frambuffer,0x00,size);
	spi_sendbytes(ILI9341_SPI_CHIP_SELECT,size, TFT_Screen_frambuffer);
}

void ili9341_fill_color_dma(short x0, short y0, short x1, short y1,int color)
{
	ili9341_set_addr_window(x0,y0,x1,y1);

	DMA_SPI_Write_test();
}

void ili9341_tests(void)
{
	//Scren tests
	get_font_header();
	//fill whole screen with black
	uint64_t start_time = get_current_time();
	ili9341_fill_color(0,0,(ili9341_get_width()-1),(ili9341_get_height()-1),COLOR_BLACK);
	calculate_execution_time(start_time);
	/*ili9341_set_vertical_scrolling_definition(0,ili9341_get_height(),0);
	ili9341_print_string_newline("This is a print test : 1",COLOR_YELLOW);
	ili9341_print_string_newline("This is a print test : 2",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 3",COLOR_BLUE);
	ili9341_print_string_newline("This is a print test : 4",COLOR_CYAN);
	ili9341_print_string_newline("This is a print test : 5",COLOR_YELLOW);
	ili9341_print_string_newline("This is a print test : 6",COLOR_BLUE);
	ili9341_print_string_newline("This is a print test : 7",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 8",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 9",COLOR_LIME);
	ili9341_print_string_newline("This is a print test : 10",COLOR_CYAN);
	ili9341_print_string_newline("This is a print test : 11",COLOR_LIME);
	ili9341_print_string_newline("This is a print test : 12",COLOR_CYAN);
	ili9341_print_string_newline("This is a print test : 13",COLOR_BLUE);
	ili9341_print_string_newline("This is a print test : 14",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 15",COLOR_YELLOW);
	ili9341_print_string_newline("This is a print test : 16",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 17",COLOR_BLUE);
	ili9341_print_string_newline("This is a print test : 18",COLOR_CYAN);
	ili9341_print_string_newline("This is a print test : 19",COLOR_WHITE);
	ili9341_print_string_newline("This is a print test : 20",COLOR_WHITE);
	ili9341_print_string_newline("This is a print test : 21",COLOR_YELLOW);
	ili9341_print_string_newline("This is a print test : 22",COLOR_RED);
	ili9341_print_string_newline("This is a print test : 23",COLOR_BLUE);
	ili9341_print_string_newline("This is a print test : 24",COLOR_CYAN);
	ili9341_print_string_newline("This is a print test : 25",COLOR_RED);*/
	
	//unsigned int current_time = get_current_time();
	//ili9341_load_image(a_image.pixel_data, a_image.width, a_image.height,a_image.bytes_per_pixel);
	//time_sleep(5000);
	/*ili9341_load_image(f_image.pixel_data, f_image.width, f_image.height,f_image.bytes_per_pixel);
	time_sleep(5000);
	ili9341_load_image(atoz_image.pixel_data, atoz_image.width, atoz_image.height,atoz_image.bytes_per_pixel);*/
	
	//ili9341_fill_color_dma(30,30,40,40,COLOR_BLACK);

	
}


