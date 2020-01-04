#include "threadpool.h"


// JAK NA RAZIE DLA CZYTELNOSCI POMINE OBSLUGIWANIE BLEDOW

/* ---- single thread waiting loop ---- */
static void *thread_loop(thread_t *t) {

  // tu mozna dodac jakies obslugiwanie sygnalow itp...


  pthread_mutex_lock(&t->pool->mutex_working_threads);
  t->pool->num_working_threads++;
  pthread_mutex_unlock(&t->pool->mutex_working_threads);

  int id = 0; // to zeby sie clion nie czepial
  while(++id < 10) {

    sem_wait(&t->pool->defer_queue->sem_not_empty);

    // sprawdzic czy nie skonczyl sie threadpool

    // tutaj skonczylem!!!!
  }




}

/* ---- thread pool ---- */
int thread_pool_init(thread_pool_t *pool, size_t num_threads) {

  if(pool == NULL) return -1;

  pool->num_threads = num_threads;
  pool->num_working_threads = 0;

  pool->threads = malloc(num_threads * sizeof(thread_t*));
  if(pool->threads == NULL) {
      return -1;
  }
  for(size_t i = 0; i < num_threads; i++) {
    pthread_create(&pool->threads[i]->pthread, NULL, (void*)thread_loop, pool->threads[i]);
    pthread_detach(pool->threads[i]->pthread);
    pool->threads[i]->pool = pool;
  }
  while(pool->num_threads_alive < num_threads) { /* czekanie na inicjalizacje */ }

  pool->defer_queue = new_defer_queue();
  if(pool->defer_queue == NULL) {
    free(pool->threads);
    return -1;
  }

  pool->not_destroyed = true;

  pthread_mutex_init(&pool->mutex_working_threads, NULL);
  pthread_cond_init(&pool->condition_idle, NULL);

  return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {

}

int defer(struct thread_pool *pool, runnable_t runnable) {


  return 0;
}
