/* System headers */
#include <stdio.h>

/* Local headers */
#include "common.h"

/* Macros */
#define MAX_TASKS 5
#define TASK_KILLER 4


/* Task Data Structure */
typedef struct Task_s 
{
    void (*f)(void *data);   /* Task function */
    void *data;             /* Private data pointer for this task */
} Task_t;


/* Globals */
Task_t tasks[MAX_TASKS];
int32_t currentTask;
uint8_t my_data[MAX_TASKS] = {10, 20, 30, 40, 50};

static uint32_t Counter = 0;


/* Function Prototypes */
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int32_t id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);
static int32_t TaskNext(void);
void cmd_task1_func(void *data);
void task_run_func(void *data);


/* Function Definitons */
void TaskCounter(void)
{
  if(Counter > 0)
  {
    TaskSwitcher();
    Counter--;
  }
}


ParserReturnVal_t Add_Task_Cmd(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  printf("CmdAddTask adding Task1\n");
  TaskAdd(cmd_task1_func, &my_data[currentTask]);

  tasks[currentTask].f(tasks[currentTask].data);

  return CmdReturnOk;
}
ADD_CMD("addtask",Add_Task_Cmd,"            Add task Command")


ParserReturnVal_t Task_Switch_Cmd(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  int32_t count = 0;
  uint32_t rc1;

  rc1 = fetch_int32_arg(&count);
  if(rc1)
  {
    printf("Count not specifed! default count: 50\r\n");
    Counter = 50;
    return CmdReturnOk;
  }

  if(count < 0)
  {
    printf("type count > 0\r\n");
  }
  else
  {
    Counter = count;
  }

  return CmdReturnOk;
}
ADD_CMD("taskswitcher",Task_Switch_Cmd,"            Task Switch Command")


int32_t TaskSwitcher(void)
{
  printf("--------------------------------> Calling Taskswitcher! count: %ld\r\n",Counter);

  currentTask = TaskNext();
  if(currentTask < 0) 
  {
    printf("No active tasks available!\n");
    return -1;
  }
  if(currentTask == 0)
  {
    currentTask = 1;
  }

  tasks[currentTask].f(tasks[currentTask].data);
  return 0;
}


int32_t TaskAdd(void (*f)(void *data), void *data)
{
    /* Try to find an empty slot */
  for(int i = 0; i < MAX_TASKS; i++) 
  {
    if(tasks[i].f == NULL)
    {
      tasks[i].f = f;
      tasks[i].data = data;
      return i;
    }

  }

  /* No slots available, return -1 */
  return -1;
}


int32_t TaskKill(int32_t id)
{
  tasks[id].f = NULL;
  tasks[id].data = NULL;

  // return something for error checking ?
  return 0;
}


/* Find the next task to run */
static int32_t TaskNext(void)
{
  int32_t i;
  uint32_t count = 0;
  i = currentTask;

  do 
  {
    i = (i + 1) % MAX_TASKS;
    count++;
  } while((tasks[i].f == NULL) && (count <= MAX_TASKS));

  return (count <= MAX_TASKS) ? i : -1;
}


void cmd_task1_func(void *data)
{
  TaskAdd(task_run_func, &my_data[currentTask + 1]);
  printf("Data: %u\nTaskId: %li\n", *((uint8_t *) data), currentTask);
}


void task_run_func(void *data)
{
  printf("Task%ld is running!\n", currentTask);

  if(currentTask == TASK_KILLER)
  {
    for(uint8_t i = 2; i <= currentTask; i++)
    {
      TaskKill(i);
      if(i == TASK_KILLER)
        printf("Task%ld killed itself with TaskId: %ld\n", currentTask, currentTask);
      else
        printf("Task%ld killed Task%d  with TaskId: %d\n", currentTask, i, i);
    }
  }
  else
  {
    TaskAdd(task_run_func, &my_data[currentTask + 1]);
    printf("Task%ld adding Task%ld\n", currentTask, currentTask + 1);
    printf("Data: %u\nTaskId: %li\n", *((uint8_t *) data), currentTask);
  }
  
}
