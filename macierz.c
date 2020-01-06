#include "threadpool.h"

void *wypiszA() {
  printf("A\n");
}

void *wypiszB() {
  printf("B\n");
}


int main() {
  thread_pool_t *pool = malloc(sizeof(thread_pool_t));
  thread_pool_init(pool, 2);

  defer(pool, (runnable_t) {
    .function = wypiszA(),
    .arg = NULL,
    .argsz = 0
  });

  thread_pool_destroy(pool);
}