#ifndef __SIMPLE_SEMAPHORE_H
#define __SIMPLE_SEMAPHORE_H

#include <stdint.h>

#define MAX_SEMAPHORES 10

typedef enum SemaphoreState_e 
{
  SEMAPHORE_FREE,
  SEMAPHORE_ALLOCATED,
} SemaphoreState_t;

/* Semaphore Data Structure */
typedef struct Semaphore_s 
{
  SemaphoreState_t state; 
  int32_t count;
  int32_t taskID;
  char *name;
} Semaphore_t;

/* Semaphore Prototypes */
void SemaphoreInit(void);
uint32_t SemaphoreNew(uint32_t *semaphoreHandle, int32_t count, char *name);
uint32_t SemaphorePost(uint32_t semaphoreHandle);
uint32_t SemaphoreAcquire(uint32_t semaphoreHandle);

#endif
