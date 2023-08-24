/* spi_expl.c:
 *
 *   Template code for establishing SPI communication with
 *   SSD1331 OLED Panel 
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"
#include "includes/ssd1331.h"


SPI_HandleTypeDef hspi1;

// Task Init
void SPI_Init(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_SPI1_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pins : SSD1331_RES_Pin SSD1331_CS_Pin SSD1331_DC_Pin 
  PA8 -> RES, PA9 -> CS, PA10 -> DC */
  GPIO_InitStruct.Pin = SSD1331_RES_Pin|SSD1331_CS_Pin|SSD1331_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /*Configure SPI pins : PA5 -> SCK, PA7 -> MOSI */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*SPI Init*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    printf("SPI init failed!");
  }

  ssd1331_init();

}

// Task Run
void SPI_Task(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
}

ADD_TASK(SPI_Task,  /* This is the name of the function for the task */
	 SPI_Init,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t SPI_Example(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */

  printf("SPI Command\n");
  ssd1331_draw_rect(0, 0, 94, 62, CYAN);

  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("spi",SPI_Example,"                SPI Command")
