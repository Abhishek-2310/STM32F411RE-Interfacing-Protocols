/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command.
 *   Also established an I2C communication protocol between a master (STM32F411RE) 
 *   and a slave (STM32L432KC).
 */

#include <stdio.h>
#include <stdint.h>

// #include "main.h"
#include "common.h"

#define I2C_ADDR 0x5A

I2C_HandleTypeDef hi2c1;

void I2CMasterInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    printf("I2C init Failed!\r\n");
  }
}

void I2CMasterTask(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
}

ADD_TASK(I2CMasterTask,  /* This is the name of the function for the task */
	 I2CMasterInit,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t I2CMasterExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, (uint8_t *)"1234", 4, HAL_MAX_DELAY) != HAL_OK)
  {
      asm("bkpt 255");
  }
  printf("I2C Master Command\n");

  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("i2c_master",I2CMasterExample,"          I2C Master Command")
