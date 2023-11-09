/**
 ******************************************************************************
 * @file    ssd1306.h
 * @author  Eng. Eletricista Andre L. A. Lopes
 * @version V1.0.0
 * @date    Terca, 7 de novembro de 2023
 * @brief   Arquivo header.
 ******************************************************************************
 * @attention
 *
 * 					e-mail: andrelopes.al@gmail.com.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SSD1306_H
#define _SSD1306_H

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "fonts.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	ssd1306_color_black = 0, /*!< Black color, no pixel */
	ssd1306_color_white      /*!< Pixel is set. Color depends on LCD */
} ssd1306_color_t;

/* Exported constants --------------------------------------------------------*/
#define SSD1306_WIDTH       (128)
#define SSD1306_HEIGHT      (64)

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
uint8_t ssd1306_init(void);

/** 
 * @brief  Updates buffer from internal RAM to LCD
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
 * @param  None
 * @retval None
 */
void ssd1306_update_screen(void);

/**
 * @brief  Toggles pixels invertion inside internal RAM
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  None
 * @retval None
 */
void ssd1306_toggle_invert(void);

/** 
 * @brief  Fills entire LCD with desired color
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_fill(ssd1306_color_t Color);

/**
 * @brief  Draws pixel at desired location
 * @note   @ref ssd1306_update_screen() must called after that in order to see updated LCD screen
 * @param  x: X location. This parameter can be a value between 0 and ssd1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and ssd1306_HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_pixel(uint16_t x, uint16_t y, ssd1306_color_t color);

/**
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and ssd1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and ssd1306_HEIGHT - 1
 * @retval None
 */
void ssd1306_goto_xy(uint16_t x, uint16_t y);

/**
 * @brief  Puts character to internal RAM
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval Character written
 */
char ssd1306_putc(char ch, FontDef_t* Font, ssd1306_color_t color);

/**
 * @brief  Puts string to internal RAM
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char ssd1306_puts(char* str, FontDef_t* Font, ssd1306_color_t color);

/**
 * @brief  Draws line on LCD
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, ssd1306_color_t c);

/**
 * @brief  Draws rectangle on LCD
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ssd1306_color_t c);

/**
 * @brief  Draws filled rectangle on LCD
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ssd1306_color_t c);

/**
 * @brief  Draws triangle on LCD
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, ssd1306_color_t color);

/**
 * @brief  Draws circle to STM buffer
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_circle(int16_t x0, int16_t y0, int16_t r, ssd1306_color_t c);

/**
 * @brief  Draws filled circle to STM buffer
 * @note   @ref ssd1306_update_screen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to ssd1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to ssd1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref ssd1306_COLOR_t enumeration
 * @retval None
 */
void ssd1306_draw_filled_circle(int16_t x0, int16_t y0, int16_t r, ssd1306_color_t c);

/**
 * @brief  Draws the Bitmap
 * @param  X:  X location to start the Drawing
 * @param  Y:  Y location to start the Drawing
 * @param  *bitmap : Pointer to the bitmap
 * @param  W : width of the image
 * @param  H : Height of the image
 * @param  color : 1-> white/blue, 0-> black
 */
void ssd1306_draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);

/**
 * @brief  Scroll screen to right
 * @retval None
 */
void ssd1306_scroll_right(uint8_t start_row, uint8_t end_row);

/**
 * @brief  Scroll screen to left
 * @retval None
 */
void ssd1306_scroll_left(uint8_t start_row, uint8_t end_row);

/**
 * @brief  Scroll screen to diagonal right
 * @retval None
 */
void ssd1306_scroll_diag_right(uint8_t start_row, uint8_t end_row);

/**
 * @brief  Scroll screen to diagonal left
 * @retval None
 */
void ssd1306_scroll_diag_left(uint8_t start_row, uint8_t end_row);

/**
 * @brief  Stop scroll screen
 * @retval None
 */
void ssd1306_stop_scroll(void);

/**
 * @brief  Invert screen
 * @retval None
 */
void ssd1306_invert_display(int i);

/**
 * @brief  Clear screen
 * @retval None
 */
void ssd1306_clear(void);

#endif /* _SSD1306_H */
