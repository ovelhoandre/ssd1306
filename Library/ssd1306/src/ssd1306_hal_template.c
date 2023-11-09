/**
 ******************************************************************************
 * @file    ssd1306_hal.c
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
#include "ssd1306_hal.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t ssd1306_i2c_init(void)
{
	return 1;
}

void ssd1306_i2c_write(uint8_t reg, uint8_t data)
{

}

void ssd1306_i2c_write_multi(uint8_t reg, uint8_t *data, uint16_t count)
{

}

void ssd1306_i2c_command(uint8_t cmd)
{
	ssd1306_i2c_write(0x00, cmd);
}

void ssd1306_i2c_data(uint8_t data)
{
	ssd1306_i2c_write(0x40, data);
}

