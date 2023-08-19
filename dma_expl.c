/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

#define ADC_BUF_LEN 4096
// #define LD2_GPIO_Port GPIOA
// #define LD2_Pin 5

// Handles
DMA_HandleTypeDef hdma_adc1_to_mem;

ADC_HandleTypeDef hadc1;

// Globals
uint8_t Buffer_Src[]={0,1,2,3,4,5,6,7,8,9};
uint8_t Buffer_Dest[10];

uint16_t adc_buf[ADC_BUF_LEN];
// void XferCpltCallback(DMA_HandleTypeDef *hdma)
// {
//     printf("Done!");
//   __NOP(); //Line reached only if transfer was successful. Toggle a breakpoint here
// }


// Interrupt handler
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1_to_mem);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

void ExampleInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */

  // DMA_ADC1
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* ADC1 DMA Init */
  hdma_adc1_to_mem.Instance = DMA2_Stream4;
  hdma_adc1_to_mem.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1_to_mem.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1_to_mem.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1_to_mem.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1_to_mem.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1_to_mem.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1_to_mem.Init.Mode = DMA_CIRCULAR;
  hdma_adc1_to_mem.Init.Priority = DMA_PRIORITY_LOW;
  hdma_adc1_to_mem.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_adc1_to_mem) != HAL_OK)
  {
    printf("DMA2 init Failed!");
  }

  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1_to_mem);

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

  // ADC
  __HAL_RCC_ADC1_CLK_ENABLE();

  /**ADC1 GPIO Configuration
  PA0-WKUP     ------> ADC1_IN0
  */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    printf("ADC init Failed!");
  }

  ADC_ChannelConfTypeDef sConfig = {0};

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    printf("ADC Channel config Failed!");
  }

}

void ExampleTask(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
}

ADD_TASK(ExampleTask,  /* This is the name of the function for the task */
	 ExampleInit,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t CmdExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  printf("DMA Example\n");

  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc_buf, 4096);

// HAL_DMA_Start(&hdma_adc1_to_mem, (uint32_t) Buffer_Src, (uint32_t) Buffer_Dest, 10);

//   if(HAL_DMA_PollForTransfer(&hdma_adc1_to_mem, HAL_DMA_FULL_TRANSFER, 100) != HAL_OK)
//   {
//     __NOP();
//   }

//   for(int i = 0; i < 10; i++)
//     printf("%d \t", Buffer_Dest[i]);
  printf("%d \t", adc_buf[100]);

  printf("\n");

  return CmdReturnOk;
}

ADD_CMD("dma",CmdExample,"                dma Command")
