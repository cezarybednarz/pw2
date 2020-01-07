#include <unistd.h>

#include "threadpool.h"


static void wypiszA(void *arg __attribute__((unused)), size_t argsz __attribute__((unused))) {
  for(int i = 0; i < 2; i++) {
    printf("A\n");
  }
}

static void wypiszB(void *arg __attribute__((unused)), size_t argsz __attribute__((unused))) {
  for(int i = 0; i < 3; i++) {
    printf("B\n");
  }
}


int main() {
/*
  defer_queue_t *q = new_defer_queue();

  runnable_t rA = (runnable_t) {
          .function = wypiszA,
          .arg = NULL,
          .argsz = 69};

  runnable_t rB = (runnable_t) {
          .function = wypiszB,
          .arg = NULL,
          .argsz = 420};

  defer_queue_push(q, &rA);
  defer_queue_push(q, &rB);

  defer_queue_pop(q);
  defer_queue_pop(q);
*/



  thread_pool_t *pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));
  thread_pool_init(pool, 2);

  runnable_t rA = (runnable_t) {.function = wypiszA,.arg = NULL,.argsz = 420};
  defer(pool, rA);

  //defer(pool, (runnable_t) {.function = wypiszB,.arg = NULL,.argsz = 69});

  sleep(2);
  printf("zaczynam destroy\n");
  thread_pool_destroy(pool);
  printf("skonczylem destroy\n");
  free(pool);

}