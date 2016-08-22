#ifndef __DISPLAY__H__
#define __DISPLAY__H__

#define TFT_DISPLAY_WIDTH 240
#define TFT_DISPLAY_HEIGHT 320


//Bits per pixel selection
#define BITS_PER_PIXEL_18
//#define BITS_PER_PIXEL_16


//Font selection
#define FONT_ARIAL_11_14
//#define TERMINAL_12_16


#if defined(BITS_PER_PIXEL_16)
	#define PIXEL_FORMAT_SET 0x55
	#define BYTES_PER_PIXEL 2
#elif defined(BITS_PER_PIXEL_18)
	#define PIXEL_FORMAT_SET 0x66
	#define BYTES_PER_PIXEL 3
#endif


#define TFT_SCREEN_BUFFER_SIZE_DMA (TFT_DISPLAY_WIDTH*TFT_DISPLAY_HEIGHT*BYTES_PER_PIXEL+4)

#define SIZE_OF_FONT_HEADER 8
#define SIZE_OF_CHAR_TABLE 4

typedef struct font_header_t{
	short font_id;
	short first_char_id;
	short last_char_id;
	short height;
} font_header_t;

typedef struct character_table_t{
	char width;
	char offset_7_0;
	char offset_15_8;
	char offset_23_16;
} character_table_t;

enum Screen_rotations
{
	ROTATION_0 = 0,
	ROTATION_1 = 1,
	ROTATION_2 = 2,
	ROTATION_3 = 3
};

enum pixel_format
{
	RGB = 0,
	BGR = 1,
};


enum COLOR_RGB
{
	COLOR_BLACK		= 	0,
 	COLOR_WHITE,			
 	COLOR_RED,		
 	COLOR_LIME,			
 	COLOR_BLUE,		
 	COLOR_YELLOW,		
 	COLOR_CYAN, 		
 	COLOR_MAGENTA, 	
 	COLOR_SILVER,		
 	COLOR_GRAY,		
 	COLOR_MAROON,		
 	COLOR_GREEN,			
};

enum ILI9341_functions
{
	ILI9341_CMD_NOP 									= 0X00,
	ILI9341_CMD_SOFTWARE_RESET 							= 0X01,
	ILI9341_CMD_READ_DISPLAY_IDENTIFICATION_INFORMATION = 0X04,
	ILI9341_CMD_READ_DISPLAY_STATUS 					= 0X09,
	ILI9341_CMD_READ_DISPLAY_POWER_MODE 				= 0X0A,
	ILI9341_CMD_READ_DISPLAY_MADCTL 					= 0X0B,
	ILI9341_CMD_READ_DISPLAY_PIXEL_FORMAT 				= 0X0C,
	ILI9341_CMD_ENTER_SLEEP_MODE 						= 0X10,
	ILI9341_CMD_SLEEP_OUT 								= 0X11,
	ILI9341_CMD_PARTIAL_MODE_ON 						= 0X12,
	ILI9341_CMD_NORMAL_DISPLAY_MODE_ON 					= 0X13,
	ILI9341_CMD_DISPLAY_INVERSION_OFF 					= 0X20,
	ILI9341_CMD_DISPLAY_INVERSION_ON 					= 0X21,
	ILI9341_CMD_DISPLAY_OFF 							= 0X28,
	ILI9341_CMD_DISPLAY_ON 								= 0X29, 
	ILI9341_CMD_COLUMN_ADDRESS_SET 						= 0X2A, 
	ILI9341_CMD_PAGE_ADDRESS_SET 						= 0X2B, 
	ILI9341_CMD_MEMORY_WRITE 							= 0X2C, 
	ILI9341_CMD_MEMORY_READ 							= 0X2E, 
	ILI9341_CMD_VERTICAL_SCROLL_DEFINITION				= 0x33,
	ILI9341_CMD_MEMORY_ACCESS_CONTROL  					= 0X36, 
	ILI9341_CMD_MEMORY_VERTICAL_SCROLL_START_ADDR		= 0X37, 
	ILI9341_CMD_IDLE_MODE_OFF 							= 0X38, 
	ILI9341_CMD_IDLE_MODE_ON 							= 0x39, 
	ILI9341_CMD_PIXEL_FORMAT_SET						= 0X3A,
	ILI9341_CMD_WRITE_MEMORY_CONTINUE 					= 0X3C, 
	ILI9341_CMD_READ_MEMORY_CONTINUE 					= 0X3E, 
	ILI9341_CMD_WRITE_DISPLAY_BRIGHTNESS 				= 0X51, 
	ILI9341_CMD_WRITE_CTRL_DISPLAY 						= 0X53, 
	ILI9341_CMD_READ_CTRL_DISPLAY 						= 0X54, 
	ILI9341_CMD_READ_ID1 								= 0XDA, 
	ILI9341_CMD_READ_ID2 								= 0XDB, 
	ILI9341_CMD_READ_ID3 								= 0XDC, 
	ILI9341_CMD_READ_ID4 								= 0XD3, 
	ILI9341_CMD_POWER_CONTROL_1 						= 0XC0, 
	ILI9341_CMD_POWER_CONTROL_2 						= 0XC1, 
	ILI9341_CMD_VCOM_CONTROL_1 							= 0XC5, 
	ILI9341_CMD_VCOM_CONTROL_2 							= 0XC7, 
};

typedef union __attribute__((packed)) _ILI9341_Mem_Access_Control_Reg_t_t
{
    struct
    {
       //LSB
       unsigned char     reserved    				:2;
	   unsigned char     MH_Hor_refresh_order   	:1;
	   unsigned char     BGR    					:1;
	   unsigned char     ML_Vert_refresh_order  	:1;
	   unsigned char     MV_Row_Column_Exchange		:1;
	   unsigned char     MX_Column_address_order	:1;
	   unsigned char     MY_Row_address_order    	:1;
       //MSB
    } mBits;  
    unsigned char   mAsU8;
} ILI9341_Mem_Access_Control_Reg_t;

void ili9341_init_tft_display(void);
int ili9341_getid(unsigned char*ptr);
unsigned short ili9341_get_width();
unsigned short ili9341_get_height();
unsigned short ili9341_get_font_height();
unsigned short ili9341_get_font_width();
int ili9341_set_screen_rotation(unsigned char rotation, int bgr);
int ili9341_set_addr_window(short x0, short y0, short x1, short y1);
int ili9341_update_display(void);
int ili9341_load_image(const unsigned char* img, int width, int height, int bytes_per_pixel);
int calculate_number_of_bytes_per_line(int width);
int ili9341_print_char(char print_char,int x, int y,int color);
int ili9341_print_string(char* print_string,int color);
int ili9341_print_string_newline(char* print_string,int color);
int ili9341_set_vertical_scrolling_definition(short top_fixed_area, short scroll_area, short bottom_fixed_area);
int ili9341_set_vertical_scrolling_set_start_addr(short start_line);
int ili9341_scroll(void);
void ili9341_fill_color(short x0, short y0, short x1, short y1,int color);
void ili9341_fill_color_dma(short x0, short y0, short x1, short y1,int color);
void ili9341_turn_display_off(void);
int ili9341_get_screen_memory(short x0, short y0, short x1, short y1);

#endif