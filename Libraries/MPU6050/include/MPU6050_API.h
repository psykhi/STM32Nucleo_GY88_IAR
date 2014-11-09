/*
 * MPU6050_API.h
 *
 *  Created on: 28 oct. 2014
 *      Author: Alexandre
 */

#ifndef MPU6050_INCLUDE_MPU6050_API_H_
#define MPU6050_INCLUDE_MPU6050_API_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "MPU6050_register_map.h"

typedef enum
{
	MPU6050_ERR_OK,
	MPU6050_ERR_TIMEOUT,
	MPU6050_ERR_NOK
}MPU6050_ErrCode;

typedef enum
{
	MPU6050_HIGH,
	MPU6050_LOW
}MPU6050_ADO;

typedef struct
{
	I2C_TypeDef* i2c_periph;
	uint32_t i2c_clk;
	uint32_t scl_pin;
	GPIO_TypeDef* scl_gpio_port;
	uint32_t scl_gpio_clk;
	uint32_t sda_pin;
	GPIO_TypeDef* sda_gpio_port;
	uint32_t sda_gpio_clk;
	uint32_t i2c_speed;
	MPU6050_ADO ado_status;
}MPU6050_InitStruct;

typedef enum
{
	MPU6050_I2C_NORMAL,
	MPU6050_I2C_FAST
}MPU6050_I2C_Mode;

MPU6050_ErrCode MPU6050_Init(MPU6050_InitStruct* i_p_init_struct );


#endif /* MPU6050_INCLUDE_MPU6050_API_H_ */
