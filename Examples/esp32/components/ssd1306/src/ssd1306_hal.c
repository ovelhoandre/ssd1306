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
#include <stdbool.h>
#include <string.h>
#include "ssd1306_hal.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_MASTER_SCL_IO           		(19)      	/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           		(18)      	/*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM             		 	(0)			/*!< I2C master i2c port number */
#define I2C_MASTER_FREQ_HZ          		(100000)	/*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   		(0)			/*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   		(0)			/*!< I2C master doesn't need buffer */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static bool _is_it_initialized = false;

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t ssd1306_i2c_init(void)
{
	esp_err_t esp_err = ESP_FAIL;

	if (_is_it_initialized == false)
	{
	    int i2c_master_port = I2C_MASTER_NUM;

	    i2c_config_t conf = {
	        .mode = I2C_MODE_MASTER,
	        .sda_io_num = I2C_MASTER_SDA_IO,
	        .scl_io_num = I2C_MASTER_SCL_IO,
	        .sda_pullup_en = GPIO_PULLUP_ENABLE,
	        .scl_pullup_en = GPIO_PULLUP_ENABLE,
	        .master.clk_speed = I2C_MASTER_FREQ_HZ,
	    };

	    i2c_param_config(i2c_master_port, &conf);

	    esp_err = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);

	    if (esp_err == ESP_OK)
	    {
	    	_is_it_initialized = true;
	    }
	}

	return 1;
}

void ssd1306_i2c_write(uint8_t reg, uint8_t data)
{
	if (_is_it_initialized == true)
	{
	    esp_err_t err = ESP_OK;

	    i2c_cmd_handle_t handle = i2c_cmd_link_create();
	    assert (handle != NULL);

	    err = i2c_master_start(handle);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write_byte(handle, SSD1306_I2C_ADDR | I2C_MASTER_WRITE, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write_byte(handle, reg, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write_byte(handle, data, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    i2c_master_stop(handle);
	    err = i2c_master_cmd_begin(I2C_MASTER_NUM, handle, SSD1306_I2C_TIMEOUT);

	end:
	    i2c_cmd_link_delete(handle);
	}
}

void ssd1306_i2c_write_multi(uint8_t reg, uint8_t *data, uint16_t count)
{
	if (_is_it_initialized == true)
	{
	    esp_err_t err = ESP_OK;

	    i2c_cmd_handle_t handle = i2c_cmd_link_create();
	    assert (handle != NULL);

	    err = i2c_master_start(handle);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write_byte(handle, SSD1306_I2C_ADDR | I2C_MASTER_WRITE, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write_byte(handle, reg, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    err = i2c_master_write(handle, data, count, true);
	    if (err != ESP_OK) {
	        goto end;
	    }

	    i2c_master_stop(handle);
	    err = i2c_master_cmd_begin(I2C_MASTER_NUM, handle, SSD1306_I2C_TIMEOUT);

	end:
	    i2c_cmd_link_delete(handle);
	}
}

void ssd1306_i2c_command(uint8_t cmd)
{
	ssd1306_i2c_write(0x00, cmd);
}

void ssd1306_i2c_data(uint8_t data)
{
	ssd1306_i2c_write(0x40, data);
}

