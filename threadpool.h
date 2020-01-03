#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>

typedef struct runnable {
  void (*function)(void *, size_t);
  void*  arg;
  size_t argsz;
} runnable_t;

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

typedef struct thread_pool {
  size_t        num_threads;
  size_t        num_free_threads;
  pthread_t**   threads;
  defer_queue_t defer_queue;
  bool          not_destroyed;
} thread_pool_t;

int thread_pool_init(thread_pool_t *pool, size_t pool_size);

void thread_pool_destroy(thread_pool_t *pool);

int defer(thread_pool_t *pool, runnable_t runnable);

#endif
