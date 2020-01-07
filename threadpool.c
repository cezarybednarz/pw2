#include "threadpool.h"

#define ERR -1

/* ---- single thread waiting loop ---- */
static void *thread_loop(void *pool) {

  printf("Zaczynam watek\n");

  thread_pool_t *p = (thread_pool_t*)pool;
  runnable_t *todo;

  while(true) {
    pthread_mutex_lock(&(p->mutex));

    while(!p->destroyed && p->defer_queue->length == 0) {
      pthread_cond_wait(&(p->condition), &(p->mutex));
    }

    if(p->destroyed && p->defer_queue->length == 0) {
      break;
    }

    defer_queue_print(p->defer_queue);

    todo = defer_queue_pop(p->defer_queue);
    pthread_mutex_unlock(&(p->mutex));
    todo->function(todo->arg, todo->argsz);
  }

  printf("Koncze watek\n");

  p->num_threads_started--;
  pthread_mutex_unlock(&(p->mutex));
  pthread_exit(NULL);
}

/* ---- thread pool ---- */
int thread_pool_init(thread_pool_t *pool, size_t num_threads) {

  if(pool == NULL) {
    return ERR;
  }

  pool->num_threads_started = 0;
  pool->destroyed = false;

  if((pool->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t))) == NULL) {
    return ERR;
  }

  if((pool->defer_queue = new_defer_queue()) == NULL) {
    free(pool->threads);
    return ERR;
  }

  if(pthread_mutex_init(&(pool->mutex), NULL) != 0) {
    free(pool->threads);
    defer_queue_destroy(pool->defer_queue);
    return ERR;
  }

  if(pthread_cond_init(&(pool->condition), NULL) != 0) {
    free(pool->threads);
    defer_queue_destroy(pool->defer_queue);
    if(pthread_mutex_destroy(&(pool->mutex)) != 0) {
      return ERR;
    }
    return ERR;
  }

  for(size_t i = 0; i < num_threads; i++) {
    if(pthread_create(&(pool->threads[i]), NULL, thread_loop, (void*)pool) != 0) {
      thread_pool_destroy(pool);
      return ERR;
    }
    pool->num_threads_started++;
  }
  return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {
  if(pool == NULL) {
    exit(ERR);
  }

  if(pthread_mutex_lock(&(pool->mutex)) != 0) {
    exit(ERR);
  }

  if(pool->destroyed) {
    exit(ERR);
  }

  pool->destroyed = true;

  if(pthread_cond_broadcast(&(pool->condition)) != 0 ||
     pthread_mutex_unlock(&(pool->mutex)) != 0) {
    exit(ERR);
  }

  for(size_t i = 0; i < pool->num_threads_started; i++) {
    if(pthread_join(pool->threads[i], NULL) != 0) {
      exit(ERR);
    }
  }

  if(pool->num_threads_started > 0) {
    exit(ERR);
  }

  free(pool->threads);
  defer_queue_destroy(pool->defer_queue);

  pthread_mutex_lock(&(pool->mutex));
  pthread_mutex_destroy(&(pool->mutex));
  pthread_cond_destroy(&(pool->condition));
}

int defer(struct thread_pool *pool, runnable_t runnable) {

  if(pool == NULL) {
    return ERR;
  }

  if(pthread_mutex_lock(&(pool->mutex)) != 0) {
    return ERR;
  }
  printf("glowny sekcja kryt\n");
  if(pool->destroyed) {
    if(pthread_mutex_unlock(&(pool->mutex)) != 0) {
      return ERR;
    }
  }
  else {
    defer_queue_push(pool->defer_queue, &runnable);

    if(pthread_cond_signal(&(pool->condition)) != 0) {
      if(pthread_mutex_unlock(&(pool->mutex)) != 0) {
        return ERR;
      }
      return ERR;
    }
    if(pthread_mutex_unlock(&(pool->mutex)) != 0) {
      return ERR;
    }
  }

  return 0;
}
