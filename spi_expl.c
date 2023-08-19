/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command.
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"

extern UART_HandleTypeDef huart2;

void SPI_Init(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */


}

void SPI_Task(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */
}

ADD_TASK(SPI_Init,  /* This is the name of the function for the task */
	 SPI_Task,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t SPI_Example(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */

  printf("SPI Command\n");

  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("spi",SPI_Example,"                SPI Command")
