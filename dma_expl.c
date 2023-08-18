/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command.
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"

DMA_HandleTypeDef hdma_memtomem;
uint8_t Buffer_Src[]={0,1,2,3,4,5,6,7,8,9};
uint8_t Buffer_Dest[10];

void XferCpltCallback(DMA_HandleTypeDef *hdma)
{
    printf("Done!");
  __NOP(); //Line reached only if transfer was successful. Toggle a breakpoint here
}

void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_memtomem);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

void ExampleInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */

  __HAL_RCC_DMA2_CLK_ENABLE();

  hdma_memtomem.Instance = DMA2_Stream0;
  hdma_memtomem.Init.Channel = DMA_CHANNEL_0;
  hdma_memtomem.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem.Init.Mode = DMA_NORMAL;
  hdma_memtomem.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_memtomem.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_memtomem.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_memtomem.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem.Init.PeriphBurst = DMA_PBURST_SINGLE;

  hdma_memtomem.XferCpltCallback = XferCpltCallback;
  if(HAL_DMA_Init(&hdma_memtomem) != HAL_OK)
  {
    printf("DMA init Failed!");
    __NOP();
  }

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  printf("Example Command\n");
  
  HAL_DMA_Start_IT(&hdma_memtomem, (uint32_t) Buffer_Src, (uint32_t) Buffer_Dest, 10);
//   if(HAL_DMA_PollForTransfer(&hdma_memtomem, HAL_DMA_FULL_TRANSFER, 100) != HAL_OK)
//   {
//     __NOP();
//   }

//   for(int i = 0; i < 10; i++)
//     printf("%d \t", Buffer_Dest[i]);

  printf("\n");

  return CmdReturnOk;
}

ADD_CMD("dma",CmdExample,"                dma Command")
