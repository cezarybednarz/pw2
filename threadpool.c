#include "threadpool.h"


// JAK NA RAZIE DLA CZYTELNOSCI POMINE OBSLUGIWANIE BLEDOW

static void *thread_loop(thread_pool_t *pool, pthread_t *thread) {

}

/* ---- thread pool ---- */
int thread_pool_init(thread_pool_t *pool, size_t num_threads) {

  if(pool == NULL) return -1;

  pool->num_threads = num_threads;
  pool->defer_queue = new_defer_queue();
  if(pool->defer_queue == NULL) {
    return -1;
  }
  pool->

  return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {

}

int defer(struct thread_pool *pool, runnable_t runnable) {
  return 0;
}
