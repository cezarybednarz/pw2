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

typedef struct node {
  runnable_t* runnable;
  node*       prev;
} node_t;

typedef struct defer_queue {
  pthread_mutex_t protection;
  sem_t           not_empty;
  node_t*         front;
  node_t*         back;
  int             length;
} defer_queue_t;

defer_queue *new_defer_queue(void);

int defer_queue_push(defer_queue_t *q);

runnable_t *defer_queue_pop(defer_queue_t *q);

int defer_queue_destroy(defer_queue_t *q);


#endif //DEFER_QUEUE_H
