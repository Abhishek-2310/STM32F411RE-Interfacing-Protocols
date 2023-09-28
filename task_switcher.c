#include <stdio.h>
#define MAX_TASKS 5

/* Globals */
Task_t tasks[MAX_TASKS];
int32_t currentTask;

/* Task Data Structure */
typedef struct Task_s 
{
    void (*f)(void *data);   /* Task function */
    void *data;             /* Private data pointer for this task */
} Task_t;


/* Function Prototypes */
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int32_t id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);


ParserReturnVal_t Task_Switch_Cmd(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  
  return CmdReturnOk;
}
ADD_CMD("taskswitcher",Task_Switch_Cmd,"            Task Switch Command")


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
