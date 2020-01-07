#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdarg.h>
#include <string.h>
#include "err.h"
#include "defer_queue.h"


typedef struct runnable {
  void (*function)(void *, size_t);
  void*  arg;
  size_t argsz;
} runnable_t;

typedef struct thread_pool {
  size_t num_threads_started;
  size_t queue_size;

  pthread_t*          threads;
  struct defer_queue* defer_queue;
  bool                destroyed;

  pthread_mutex_t  mutex;
  pthread_cond_t   condition;

} thread_pool_t;

int thread_pool_init(thread_pool_t *pool, size_t pool_size);

void thread_pool_destroy(thread_pool_t *pool);

int defer(thread_pool_t *pool, runnable_t runnable);

#endif // THREADPOOL_H
