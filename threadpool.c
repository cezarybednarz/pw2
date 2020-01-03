#include "threadpool.h"


// JAK NA RAZIE DLA CZYTELNOSCI POMINE OBSLUGIWANIE BLEDOW

/* ---- error handling ---- */
void syserr(int bl, const char *fmt, ...) {
  va_list fmt_args;
  fprintf(stderr, "ERROR: ");
  va_start(fmt_args, fmt);
  vfprintf(stderr, fmt, fmt_args);
  va_end (fmt_args);
  fprintf(stderr," (%d; %s)\n", bl, strerror(bl));
  exit(1);
}

/* ---- defer queue ---- */
static int defer_queue_init(defer_queue_t *q) {

  return 0;
}

static int defer_queue_push(defer_queue_t *q) {

  return 0;
}

static runnable_t *defer_queue_pop(defer_queue_t *q) {

}

static int defer_queue_destroy(defer_queue_t *q) {

  return 0;
}

/* ---- thread pool ---- */
int thread_pool_init(thread_pool_t *pool, size_t num_threads) {
  return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {

}

int defer(struct thread_pool *pool, runnable_t runnable) {
  return 0;
}
