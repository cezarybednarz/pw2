#ifndef DEFER_QUEUE_H
#define DEFER_QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>
#include "err.h"
#include "threadpool.h"

typedef struct runnable runnable_t;
typedef struct node node_t;

typedef struct node {
  runnable_t*  runnable;
  node_t*      prev;
} node_t;

typedef struct defer_queue {
  sem_t           sem_not_empty;
  node_t*         front;
  node_t*         back;
  int             length;
} defer_queue_t;

defer_queue *new_defer_queue(void);

int defer_queue_push(defer_queue_t *q, runnable_t *runnable);

runnable_t *defer_queue_pop(defer_queue_t *q);

int defer_queue_destroy(defer_queue_t *q);


#endif //DEFER_QUEUE_H
