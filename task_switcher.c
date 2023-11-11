/* System headers */
#include <stdio.h>

/* Local headers */
#include "common.h"
#include "simple_semaphore.h"

/* Macros */
#define MAX_TASKS 5

/* ENUMS */
typedef enum {
  TASK_READY = 0,
  TASK_PENDING,
  TASK_RUNNING,
  TASK_INACTIVE,
  TASK_INTERRUPTED,
  TASK_STATE_MAX
} TaskState_e;

/* Task Data Structure */
typedef struct Task_s 
{
    void (*f)(void *data);   /* Task function */
    void *data;             /* Private data pointer for this task */
    TaskState_e state;
} Task_t;

/* External Globals */
extern Semaphore_t Semaphores[MAX_SEMAPHORES];

/* Globals */
Task_t tasks[MAX_TASKS];

int32_t currentTask;
static uint32_t Counter = 0;
uint8_t runCounter = 0;
uint8_t CmdTaskAdded = 0;
uint8_t my_data[MAX_TASKS] = {10, 20, 30, 40, 50}; 

/* Function Prototypes */
int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int32_t id);
int32_t TaskSwitcher(void);
static int32_t TaskNext(void);

int32_t TaskCurrent(void);
int32_t TaskPending(int32_t id); //new fn to Set task state to PENDING
int32_t TaskReady(int32_t id); //new fn to Set task state to READY

void cmd_task1_func(void *data);
void task_run_func(void *data);


/* Function Definitons */
void TaskCounter(void)
{
  if(Counter > 0)
  {
    if(TaskSwitcher() < 0)
      printf("No active tasks available!\n");
    
    Counter--;
  }
}


ParserReturnVal_t Add_Task_Cmd(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  if(CmdTaskAdded == 0)
  {
    SemaphoreInit();
    CmdTaskAdded = 1;
    printf("CmdAddTask adding Task1\n");
    TaskAdd(cmd_task1_func, &my_data[currentTask]);

    tasks[currentTask].f(tasks[currentTask].data);
  }
  else
  {
    int32_t newTaskId = TaskAdd(task_run_func, &my_data[currentTask + 1]);
    if(newTaskId < 0)
    {
      printf("No slot to add new task!\r\n");
      return CmdReturnOk;
    }

    printf("CmdAddTask adding Task%ld\n", newTaskId);
    printf("Data: %u\nTaskId: %li\n", my_data[currentTask], currentTask);
  }

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
    printf("Count not specifed! default count: 1\r\n");
    Counter = 1;
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
    currentTask = 0;
    return -1;
  }
  if(currentTask == 0)
  {
    if(TaskNext() == 0)
    {
      currentTask = 0;
      return -1;
    }
    else
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
      tasks[i].state = TASK_READY;
      return i;
    }

  }

  /* No slots available, return -1 */
  return -1;
}

int32_t TaskKill(int32_t id)
{
  if(tasks[id].state == TASK_RUNNING)
  {
    tasks[id].state = TASK_INACTIVE;
    return 0;
  }

  return -1;
}

char * getState(uint8_t task_state)
{
  switch(task_state)
  {
    case TASK_RUNNING: return "TASK_RUNNING";
    case TASK_READY: return "TASK_READY";
    case TASK_INACTIVE: return "TASK_INACTIVE";
    case TASK_PENDING: return "TASK_PENDING";
  }

  return "NULL";
}

int32_t TaskCurrent(void)
{
  return currentTask;
}

int32_t TaskPending(int32_t id)
{
  printf("Changing Task%ld state to TASK_PENDING\r\n", id);
  if(tasks[id].state == TASK_PENDING)
  {
    printf("\tFailed: Task state is already TASK_PENDING\n");
    return -1;
  }

  tasks[id].state = TASK_PENDING;

  return 0;

}

int32_t TaskReady(int32_t id)
{

  switch(tasks[id].state)
  {
    case TASK_RUNNING:
      printf("Changing Task%ld state to TASK_READY\r\n", id);
      break;
    case TASK_PENDING:
      printf("Changing Task%ld state from TASK_PENDING to TASK_READY\n",id);
      break;
    case TASK_INACTIVE:
      printf("Changing Task%ld state from TASK_INACTIVE to TASK_READY\n",id);
      break;

    default:
      printf("Changing Task%ld state to TASK_READY\r\n", id);
      printf("\tFailed: Task state is already TASK_READY\n");
      return -1;
      break;
  }

  tasks[id].state = TASK_READY;
  
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
  } while(((tasks[i].f == NULL) && (tasks[i].state != TASK_READY) 
          && (count <= MAX_TASKS)) || (tasks[i].state == TASK_PENDING));

  return (count <= MAX_TASKS) ? i : -1;
}


void cmd_task1_func(void *data)
{
  TaskAdd(task_run_func, &my_data[currentTask + 1]);
  printf("\tData: %u\n\tTaskId: %li\n\tState: %s\n", *((uint8_t *) data), currentTask, getState(tasks[currentTask + 1].state));
}


void task_run_func(void *data)
{
  tasks[currentTask].state = TASK_RUNNING;
  printf("Changing Task%ld state to TASK_RUNNING\r\n", currentTask);

  int32_t newTaskId;
  static uint32_t uSemaphoreHandle;

  switch (currentTask)
  {
    case 1:

      newTaskId = TaskAdd(task_run_func, &my_data[currentTask + 1]);
      if(newTaskId < 0)
      {
        switch (runCounter)
        {
          case 1:
            SemaphoreNew(&uSemaphoreHandle, 1, "Sem1");
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
            }
            break;

          case 2:
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
            }
            break;

          case 4:
            printf("Action: Posting Semaphore\n");
            if(SemaphorePost(uSemaphoreHandle) == 0)
            {
              printf("\tPosted Semaphore Succesfully\n");
            }
            break;

          case 5:
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
            }
            break;

          case 7:
            printf("Action: Posting Semaphore\n");
            if(SemaphorePost(uSemaphoreHandle) == 0)
            {
              printf("\tPosted Semaphore Succesfully\n");
            }
            break;

          default:
            break;
        }
      }
      else
      {
        printf("Task%ld adding Task%ld\n", currentTask, newTaskId);
        printf("\tData: %u\n\tTaskId: %li\n\tState: %s\n", *((uint8_t *) data), currentTask, getState(tasks[currentTask + 1].state));
      }
      TaskReady(1);
      break;

    case 2:
      newTaskId = TaskAdd(task_run_func, &my_data[currentTask + 1]);
      if(newTaskId < 0)
      {
        switch (runCounter)
        {
          case 1:
            printf("Action: Posting Semaphore\n");
            if(SemaphorePost(uSemaphoreHandle) == 0)
            {
              printf("\tPosted Semaphore Succesfully\n");
              TaskReady(2);
            }
            break;

          case 2:
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
              TaskReady(2);
            }
            else
            {
              printf("\tFailed to acquire Semaphore\n");
            }
            break;

          case 4:
            printf("\tTask2 acquired semaphore when Task1 posted,"
                    " as it was waiting for this semaphore\n");
            TaskReady(2); 
            break;

          case 5:
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
              TaskReady(2);
            }
            else
            {
              printf("\tFailed to acquire Semaphore\n");
            }
            break;

          case 7:
            printf("\tTask2 acquired semaphore when Task1 posted,"
                    " as it was waiting for this semaphore\n");
            TaskReady(2); 
            break;

          default:
            break;
        }
       
      }
      else
      {
        printf("Task%ld adding Task%ld\n", currentTask, newTaskId);
        printf("\tData: %u\n\tTaskId: %li\n\tState: %s\n", *((uint8_t *) data), currentTask, getState(tasks[currentTask + 1].state));
        TaskReady(2);
      }
      break;

    case 3:

      newTaskId = TaskAdd(task_run_func, &my_data[currentTask + 1]);
      if(newTaskId < 0)
      {
        switch (runCounter)
        {
          case 1:
            break;

          case 2:
            break;

          case 5:
            printf("Action: Acquiring Semaphore\n");
            if(SemaphoreAcquire(uSemaphoreHandle) == 0)
            {
              printf("\tAcquired Semaphore Succesfully\n");
              TaskReady(2);
            }
            else
            {
              printf("\tFailed to acquire Semaphore\n");
            }
            break;
        
          default:
            break;
        }
      }
      else
      {
        printf("Task%ld adding Task%ld\n", currentTask, newTaskId);
        printf("\tData: %u\n\tTaskId: %li\n\tState: %s\n", *((uint8_t *) data), currentTask, getState(tasks[currentTask + 1].state));
      }
      
      TaskReady(3);
      break;

    case 4:

      switch (runCounter)
      {
        case 1:
          break;

        case 2:
          break;

        case 4:
          printf("Action: Posting Semaphore\n");
          if(SemaphorePost(uSemaphoreHandle) == 0)
          {
            printf("\tPosted Semaphore Succesfully\n");
          }
          break;
      
        case 7:
          printf("Action: Posting Semaphore\n");
          if(SemaphorePost(uSemaphoreHandle) == 0)
          {
            printf("\tPosted Semaphore Succesfully\n");
          }
          break;

        default:
          break;
      }
      runCounter++;
      TaskReady(4);
      break;
  
    default:
      break;
  }
}