#include <stdio.h>
#include <stdint.h>

#include "common.h"

static uint32_t Counter = 0;
uint8_t led_flag = 0;         // this flag is used to stop/interrupt 
                              // the count 

void TaskCounter(void)
{
  if(Counter == 0)
  {
    led_flag = 0;
  }
  else if(Counter > 0)
  {
    if(led_flag)
    {
      printf("count value: %ld\r\n",Counter--);
    }
  }
}

void LED_Init(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = 0;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

void LED_Task(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
  if(led_flag)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(50);
  }
  else
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

}

ADD_TASK(LED_Task,  /* This is the name of the function for the task */
	 LED_Init,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the LED task")


ParserReturnVal_t CountExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */

  int32_t addcount = 0;
  uint32_t rc1;

  rc1 = fetch_int32_arg(&addcount);
  if(rc1)
  {
    printf("Specify count!\r\n");
    return CmdReturnBadParameter1;
  }

  // check for negative values
  if(addcount < 0)
  {
    printf("type num > 0\r\n");
  }
  else
  {
    Counter = addcount;
    led_flag = 1;
  }
  
  return CmdReturnOk;
}

ADD_CMD("count",CountExample,"               Count Command")

// To stop the count and reset it
ParserReturnVal_t StopCount(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  led_flag = 0;
  Counter = 0;

  return CmdReturnOk;
}

ADD_CMD("s",StopCount,"               Stop Count Command")