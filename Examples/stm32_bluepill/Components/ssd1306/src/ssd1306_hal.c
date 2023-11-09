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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t ssd1306_i2c_init(void)
{
	/* Check if LCD connected to I2C */
	if (HAL_I2C_IsDeviceReady(&hi2c1, SSD1306_I2C_ADDR, 1, SSD1306_I2C_TIMEOUT) != HAL_OK)
	{
		/* Return false */
		return 0;
	}
	
	return 1;
}

void ssd1306_i2c_write(uint8_t reg, uint8_t data)
{
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, dt, 2, 10);
}

void ssd1306_i2c_write_multi(uint8_t reg, uint8_t *data, uint16_t count)
{
	uint8_t dt[256];
	dt[0] = reg;
	uint8_t i;
	for(i = 0; i < count; i++)
	{
		dt[i+1] = data[i];
	}
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, dt, count+1, 10);
}

void ssd1306_i2c_command(uint8_t cmd)
{
	ssd1306_i2c_write(0x00, cmd);
}

void ssd1306_i2c_data(uint8_t data)
{
	ssd1306_i2c_write(0x40, data);
}

