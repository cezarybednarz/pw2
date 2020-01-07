#include "threadpool.h"

#define ERR -1

/* ---- single thread waiting loop ---- */
static void *thread_loop(void *pool) {

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

    todo = (runnable_t*)queue_pop(p->defer_queue);
    pthread_mutex_unlock(&(p->mutex));
    todo->function(todo->arg, todo->argsz);
    free(todo);
  }

  p->num_threads_started--;
  pthread_mutex_unlock(&(p->mutex));
  pthread_exit(NULL);
}

/* ---- sigaction handling ---- */
static void clear_pools(int sig_id __attribute__((unused))) {
  printf("xd");
}

/* ---- thread pool ---- */
int thread_pool_init(thread_pool_t *pool, size_t num_threads) {

  if(pool == NULL) {
    return ERR;
  }

  struct sigaction sig_act;
  sigemptyset(&sig_act.sa_mask);
  sig_act.sa_flags = 0;
  sig_act.sa_handler = clear_pools;
  if(sigaction(SIGINT, &sig_act, NULL) != 0) {
    return ERR;
  }


  pool->num_threads_started = 0;
  pool->num_threads = 0;
  pool->destroyed = false;

  if((pool->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t))) == NULL) {
    return ERR;
  }

  if((pool->defer_queue = new_queue()) == NULL) {
    free(pool->threads);
    return ERR;
  }

  if(pthread_mutex_init(&(pool->mutex), NULL) != 0) {
    free(pool->threads);
    queue_destroy(pool->defer_queue);
    return ERR;
  }

  if(pthread_cond_init(&(pool->condition), NULL) != 0) {
    free(pool->threads);
    queue_destroy(pool->defer_queue);
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
    pool->num_threads++;
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

  for(size_t i = 0; i < pool->num_threads; i++) {
    if(pthread_join(pool->threads[i], NULL) != 0) {
      exit(ERR);
    }
  }

  if(pool->num_threads_started > 0) {
    exit(ERR);
  }

  free(pool->threads);
  queue_destroy(pool->defer_queue);

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
  if(pool->destroyed) {
    if(pthread_mutex_unlock(&(pool->mutex)) != 0) {
      return ERR;
    }
  }
  else {
    runnable_t *new_runnable = malloc(sizeof(runnable));
    new_runnable->function = runnable.function;
    new_runnable->arg = runnable.arg;
    new_runnable->argsz = runnable.argsz;
    queue_push(pool->defer_queue, (void*)new_runnable);

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
