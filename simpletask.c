#include <stdio.h>
#include <stdint.h>

#include "common.h"

static uint32_t Counter = 0;

void TaskCounter(void)
{
  if(Counter > 0)
    printf("count value: %ld\r\n",Counter--);
  
  // HAL_Delay(1000);
}

ParserReturnVal_t CountExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  // uint8_t data[] = "HELLO WORLD \r\n";
  // HAL_UART_Transmit(&huart2, (uint8_t *) data, sizeof(data), 10);
  int32_t addcount = 0;
  uint32_t rc1;

  rc1 = fetch_int32_arg(&addcount);
  if(rc1)
  {
    printf("Specify count!\r\n");
    return CmdReturnBadParameter1;
  }

  if(addcount < 0)
    printf("Oombiya value\r\n");
  else
    Counter += addcount;


  // HAL_Delay(250);

  return CmdReturnOk;
}

ADD_CMD("count",CountExample,"               Count Command")