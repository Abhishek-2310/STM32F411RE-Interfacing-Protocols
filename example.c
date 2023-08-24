/* example.c:
 *
 *   Template code for implementing a simple task, using the ADD_TASK()
 *   macro.  Also contains template code for a simple monitor command. This also 
 *   implements a UART data transmit.
 * 
 *   Note: 
 *   When probing the TX/RX pins, choose the ones near to the USB port as the 
 *   PA2/PA3 are disabled by default.
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "common.h"

extern UART_HandleTypeDef huart2;

void ExampleInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */


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
  uint8_t data[] = "HELLO WORLD \r\n";
  HAL_UART_Transmit(&huart2, (uint8_t *) data, sizeof(data), 10);

  printf("Example Command\n");

  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("example",CmdExample,"                Example Command")
