/**
 ******************************************************************************
 * @file    ssd1306_hal.h
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
#ifndef _SSD1306_HAL_H
#define _SSD1306_HAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SSD1306_I2C_ADDR	(0x78)
#define SSD1306_I2C_TIMEOUT	(20000)

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
uint8_t ssd1306_i2c_init(void);

/**
 * @brief  Writes single byte to slave
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void ssd1306_i2c_write(uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void ssd1306_i2c_write_multi(uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Writes a command
 * @param  cmd: command to be written
 * @retval None
 */
void ssd1306_i2c_command(uint8_t cmd);

/**
 * @brief  Writes a data
 * @param  data: data to be written
 * @retval None
 */
void ssd1306_i2c_data(uint8_t data);

#endif /* _SSD1306_HAL_H */
