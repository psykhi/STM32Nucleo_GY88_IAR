/*
 * MPU6050.c
 *
 *  Created on: 28 oct. 2014
 *      Author: Alexandre
 */


#include "MPU6050_API.h"
#include "MPU6050_const.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
/*******************STATIC FUNCTIONS**********/

static MPU6050_ErrCode MPU6050_write_reg(
		MPU6050_Reg_address i_address,uint8_t i_value);
static MPU6050_ErrCode MPU6050_read_reg(
		MPU6050_Reg_address i_address,
		uint8_t* o_buffer);

/*************************************TYPES***********************/
typedef struct
{
	I2C_TypeDef* I2C_periph;
	uint16_t timeout;
}MPU6050_context;
/****************************LOCAL VARIABLES*************/
static MPU6050_context context;

/*****************************IMPLEMENTATION*******/


static void MPU6050_I2C_init(MPU6050_InitStruct* i_p_init_struct);

MPU6050_ErrCode MPU6050_Init(MPU6050_InitStruct* i_p_init_struct )
{
	MPU6050_ErrCode err = MPU6050_ERR_OK;

	MPU6050_I2C_init(i_p_init_struct);
	/* Now the low-level stuff is done, we can start preparing the device itself*/

	return err;
}

static void MPU6050_I2C_init(MPU6050_InitStruct* i_p_init_struct)
{
	GPIO_InitTypeDef gpio_init;
	I2C_InitTypeDef i2c_init;

	/* We must first initialize the peripheral clock*/
	RCC_APB1PeriphClockCmd(i_p_init_struct->i2c_clk,ENABLE);

	/* Then the SDA,SDL GPIO pins */
	RCC_APB2PeriphClockCmd(
			i_p_init_struct->scl_gpio_clk |
			i_p_init_struct->sda_gpio_clk,ENABLE);

	/*SCL*/
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Pin = i_p_init_struct->scl_pin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(i_p_init_struct->scl_gpio_port,&gpio_init);

	/*SDA*/
	gpio_init.GPIO_Pin = i_p_init_struct->sda_pin;
	GPIO_Init(i_p_init_struct->sda_gpio_port,&gpio_init);

	i2c_init.I2C_Ack = I2C_Ack_Enable;
	i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init.I2C_ClockSpeed = i_p_init_struct->i2c_speed;
	i2c_init.I2C_DutyCycle = I2C_DutyCycle_16_9;
	i2c_init.I2C_Mode = I2C_Mode_I2C;
	i2c_init.I2C_OwnAddress1 = 0x00;

	I2C_Init(i_p_init_struct->i2c_periph,&i2c_init);

	I2C_Cmd(i_p_init_struct->i2c_periph,ENABLE);

	context.I2C_periph = i_p_init_struct->i2c_periph;

}
static MPU6050_ErrCode MPU6050_write_reg(
		MPU6050_Reg_address i_address,uint8_t i_value)
{
	MPU6050_ErrCode err = MPU6050_ERR_OK;

	return err;
}

static MPU6050_ErrCode MPU6050_read_reg(
		MPU6050_Reg_address i_address,
		uint8_t* o_buffer)
{
	MPU6050_ErrCode err = MPU6050_ERR_OK;


	/* First, we must wait for the device to be available
	 * ie not busy.
	 */
	context.timeout = MPU6050_I2C_TIMEOUT;
	while (I2C_GetFlagStatus(context.I2C_periph,I2C_FLAG_BUSY))
	{
		if((context.timeout--) == 0) return MPU6050_ERR_TIMEOUT;
	}

	I2C_GenerateSTART(context.I2C_periph,ENABLE);
	/* Then we wait for the SB flag*/
	context.timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_GetFlagStatus(context.I2C_periph,I2C_FLAG_SB))
	{
		if((context.timeout--) == 0) return MPU6050_ERR_TIMEOUT;
	}


	I2C_GenerateSTOP(context.I2C_periph,ENABLE);

	return err;
}
