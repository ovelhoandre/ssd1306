/**
 ******************************************************************************
 * @file    ssd1306.c
 * @author  Eng. Eletricista Andre L. A. Lopes
 * @version V1.0.0
 * @date    Terca, 7 de novembro de 2023
 * @brief   Arquivo source.
 ******************************************************************************
 * @attention
 *
 * 					e-mail: andrelopes.al@gmail.com.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ssd1306.h"
#include "ssd1306_hal.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16_t current_x;
	uint16_t current_y;
	uint8_t inverted;
	uint8_t initialized;
} ssd1306_work_t;

/* Private define ------------------------------------------------------------*/
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              (0x26)
#define SSD1306_LEFT_HORIZONTAL_SCROLL               (0x27)
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL (0x29)
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  (0x2A)
#define SSD1306_DEACTIVATE_SCROLL                    (0x2E) // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      (0x2F) // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             (0xA3) // Set scroll range
#define SSD1306_NORMALDISPLAY						 (0xA6)
#define SSD1306_INVERTDISPLAY						 (0xA7)

/* Private macro -------------------------------------------------------------*/
#define ABS(x) ((x) > 0 ? (x) : -(x))

/* Private variables ---------------------------------------------------------*/
static uint8_t ssd1306_buffer[(SSD1306_WIDTH * SSD1306_HEIGHT) / 8];
static ssd1306_work_t ssd1306_work;

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t ssd1306_init(void)
{
	/* Init I2C */
	if (ssd1306_i2c_init() == 0)
	{
		return 0;
	}

	/* A little delay */
	uint32_t p = 2500;
	while(p>0) p--;

	/* Init LCD */
	ssd1306_i2c_command(0xAE); //display off
	ssd1306_i2c_command(0x20); //Set Memory Addressing Mode
	ssd1306_i2c_command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_i2c_command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_i2c_command(0xC8); //Set COM Output Scan Direction
	ssd1306_i2c_command(0x00); //---set low column address
	ssd1306_i2c_command(0x10); //---set high column address
	ssd1306_i2c_command(0x40); //--set start line address
	ssd1306_i2c_command(0x81); //--set contrast control register
	ssd1306_i2c_command(0xFF);
	ssd1306_i2c_command(0xA1); //--set segment re-map 0 to 127
	ssd1306_i2c_command(0xA6); //--set normal display
	ssd1306_i2c_command(0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_i2c_command(0x3F); //
	ssd1306_i2c_command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_i2c_command(0xD3); //-set display offset
	ssd1306_i2c_command(0x00); //-not offset
	ssd1306_i2c_command(0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_i2c_command(0xF0); //--set divide ratio
	ssd1306_i2c_command(0xD9); //--set pre-charge period
	ssd1306_i2c_command(0x22); //
	ssd1306_i2c_command(0xDA); //--set com pins hardware configuration
	ssd1306_i2c_command(0x12);
	ssd1306_i2c_command(0xDB); //--set vcomh
	ssd1306_i2c_command(0x20); //0x20,0.77xVcc
	ssd1306_i2c_command(0x8D); //--set DC-DC enable
	ssd1306_i2c_command(0x14); //
	ssd1306_i2c_command(0xAF); //--turn on ssd1306_work panel

	ssd1306_i2c_command(SSD1306_DEACTIVATE_SCROLL);

	/* Clear screen */
	ssd1306_fill(ssd1306_color_black);

	/* Update screen */
	ssd1306_update_screen();

	/* Set default values */
	ssd1306_work.current_x = 0;
	ssd1306_work.current_y = 0;

	/* Initialized OK */
	ssd1306_work.initialized = 1;

	return 1;
}

void ssd1306_update_screen(void)
{
	uint8_t m;

	for (m = 0; m < 8; m++)
	{
		ssd1306_i2c_command(0xB0 + m);
		ssd1306_i2c_command(0x00);
		ssd1306_i2c_command(0x10);

		/* Write multi data */
		ssd1306_i2c_write_multi(0x40, &ssd1306_buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

void ssd1306_scroll_right(uint8_t start_row, uint8_t end_row)
{
	ssd1306_i2c_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(start_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(end_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(0XFF);
	ssd1306_i2c_command(SSD1306_ACTIVATE_SCROLL);
}

void ssd1306_scroll_left(uint8_t start_row, uint8_t end_row)
{
	ssd1306_i2c_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(start_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(end_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(0XFF);
	ssd1306_i2c_command(SSD1306_ACTIVATE_SCROLL);
}

void ssd1306_scroll_diag_right(uint8_t start_row, uint8_t end_row)
{
	ssd1306_i2c_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(SSD1306_HEIGHT);
	ssd1306_i2c_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(start_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(end_row);
	ssd1306_i2c_command(0x01);
	ssd1306_i2c_command(SSD1306_ACTIVATE_SCROLL);
}

void ssd1306_scroll_diag_left(uint8_t start_row, uint8_t end_row)
{
	ssd1306_i2c_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(SSD1306_HEIGHT);
	ssd1306_i2c_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	ssd1306_i2c_command(0x00);
	ssd1306_i2c_command(start_row);
	ssd1306_i2c_command(0X00);
	ssd1306_i2c_command(end_row);
	ssd1306_i2c_command(0x01);
	ssd1306_i2c_command(SSD1306_ACTIVATE_SCROLL);
}

void ssd1306_stop_scroll(void)
{
	ssd1306_i2c_command(SSD1306_DEACTIVATE_SCROLL);
}

void ssd1306_invert_display (int i)
{
	if (i)
	{
		ssd1306_i2c_command(SSD1306_INVERTDISPLAY);
	}
	else
	{
		ssd1306_i2c_command(SSD1306_NORMALDISPLAY);
	}
}

void ssd1306_draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for (int16_t j=0; j<h; j++, y++)
    {
        for (int16_t i=0; i<w; i++)
        {
            if (i & 7)
            {
            	byte <<= 1;
            }
            else
            {
            	byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }
            if (byte & 0x80)
            {
            	ssd1306_draw_pixel(x+i, y, !color);
            }
            else
            {
            	ssd1306_draw_pixel(x+i, y, color);
            }
        }
    }
}

void ssd1306_toggle_invert(void)
{
	uint16_t i;
	
	/* Toggle invert */
	ssd1306_work.inverted = !ssd1306_work.inverted;
	
	/* Do memory toggle */
	for (i = 0; i < sizeof(ssd1306_buffer); i++)
	{
		ssd1306_buffer[i] = ~ssd1306_buffer[i];
	}
}

void ssd1306_fill(ssd1306_color_t color)
{
	memset(ssd1306_buffer, (color == ssd1306_color_black) ? 0x00 : 0xFF, sizeof(ssd1306_buffer));
}

void ssd1306_draw_pixel(uint16_t x, uint16_t y, ssd1306_color_t color)
{
	if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT))
	{
		return;
	}
	
	/* Check if pixels are inverted */
	if (ssd1306_work.inverted)
	{
		color = (ssd1306_color_t)!color;
	}
	
	/* Set color */
	if (color == ssd1306_color_white)
	{
		ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void ssd1306_goto_xy(uint16_t x, uint16_t y)
{
	ssd1306_work.current_x = x;
	ssd1306_work.current_y = y;
}

char ssd1306_putc(char ch, FontDef_t* Font, ssd1306_color_t color)
{
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if ((SSD1306_WIDTH <= (ssd1306_work.current_x + Font->FontWidth)) || (SSD1306_HEIGHT <= (ssd1306_work.current_y + Font->FontHeight)))
	{
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++)
	{
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				ssd1306_draw_pixel(ssd1306_work.current_x + j, (ssd1306_work.current_y + i), (ssd1306_color_t) color);
			}
			else
			{
				ssd1306_draw_pixel(ssd1306_work.current_x + j, (ssd1306_work.current_y + i), (ssd1306_color_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	ssd1306_work.current_x += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

char ssd1306_puts(char* str, FontDef_t* Font, ssd1306_color_t color)
{
	/* Write characters */
	while (*str)
	{
		/* Write character by character */
		if (ssd1306_putc(*str, Font, color) != *str)
		{
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}
 
void ssd1306_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, ssd1306_color_t c)
{
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH)
	{
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH)
	{
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT)
	{
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT)
	{
		y1 = SSD1306_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0)
	{
		if (y1 < y0)
		{
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0)
		{
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++)
		{
			ssd1306_draw_pixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0)
	{
		if (y1 < y0)
		{
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0)
		{
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++)
		{
			ssd1306_draw_pixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1)
	{
		ssd1306_draw_pixel(x0, y0, c);
		if (x0 == x1 && y0 == y1)
		{
			break;
		}
		e2 = err; 
		if (e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		} 
	}
}

void ssd1306_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ssd1306_color_t c)
{
	/* Check input parameters */
	if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT))
	{
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH)
	{
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT)
	{
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	ssd1306_draw_line(x, y, x + w, y, c);         /* Top line */
	ssd1306_draw_line(x, y + h, x + w, y + h, c); /* Bottom line */
	ssd1306_draw_line(x, y, x, y + h, c);         /* Left line */
	ssd1306_draw_line(x + w, y, x + w, y + h, c); /* Right line */
}

void ssd1306_draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ssd1306_color_t c)
{
	uint8_t i;
	
	/* Check input parameters */
	if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT))
	{
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH)
	{
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT)
	{
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++)
	{
		/* Draw lines */
		ssd1306_draw_line(x, y + i, x + w, y + i, c);
	}
}

void ssd1306_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, ssd1306_color_t color)
{
	/* Draw lines */
	ssd1306_draw_line(x1, y1, x2, y2, color);
	ssd1306_draw_line(x2, y2, x3, y3, color);
	ssd1306_draw_line(x3, y3, x1, y1, color);
}

void ssd1306_draw_filled_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, ssd1306_color_t color)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1)
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)
	{
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else
	{
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		ssd1306_draw_line(x, y, x3, y3, color);

		num += numadd;
		if (num >= den)
		{
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void ssd1306_draw_circle(int16_t x0, int16_t y0, int16_t r, ssd1306_color_t c)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ssd1306_draw_pixel(x0, y0 + r, c);
    ssd1306_draw_pixel(x0, y0 - r, c);
    ssd1306_draw_pixel(x0 + r, y0, c);
    ssd1306_draw_pixel(x0 - r, y0, c);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ssd1306_draw_pixel(x0 + x, y0 + y, c);
        ssd1306_draw_pixel(x0 - x, y0 + y, c);
        ssd1306_draw_pixel(x0 + x, y0 - y, c);
        ssd1306_draw_pixel(x0 - x, y0 - y, c);

        ssd1306_draw_pixel(x0 + y, y0 + x, c);
        ssd1306_draw_pixel(x0 - y, y0 + x, c);
        ssd1306_draw_pixel(x0 + y, y0 - x, c);
        ssd1306_draw_pixel(x0 - y, y0 - x, c);
    }
}

void ssd1306_draw_filled_circle(int16_t x0, int16_t y0, int16_t r, ssd1306_color_t c)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ssd1306_draw_pixel(x0, y0 + r, c);
    ssd1306_draw_pixel(x0, y0 - r, c);
    ssd1306_draw_pixel(x0 + r, y0, c);
    ssd1306_draw_pixel(x0 - r, y0, c);
    ssd1306_draw_line(x0 - r, y0, x0 + r, y0, c);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ssd1306_draw_line(x0 - x, y0 + y, x0 + x, y0 + y, c);
        ssd1306_draw_line(x0 + x, y0 - y, x0 - x, y0 - y, c);

        ssd1306_draw_line(x0 + y, y0 + x, x0 - y, y0 + x, c);
        ssd1306_draw_line(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}

void ssd1306_clear (void)
{
	ssd1306_fill(0);
    ssd1306_update_screen();
}

void ssd1306_on(void)
{
	ssd1306_i2c_command(0x8D);
	ssd1306_i2c_command(0x14);
	ssd1306_i2c_command(0xAF);
}

void ssd1306_off(void)
{
	ssd1306_i2c_command(0x8D);
	ssd1306_i2c_command(0x10);
	ssd1306_i2c_command(0xAE);
}

