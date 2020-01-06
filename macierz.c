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
  thread_pool_t pool;
  thread_pool_init(&pool, 2);

  defer(&pool, (runnable_t) {
    .function = wypiszA,
    .arg = NULL,
    .argsz = 0
  });

  defer(&pool, (runnable_t) {
    .function = wypiszB,
    .arg = NULL,
    .argsz = 0
  });


  printf("zaczynam destroy\n");
  thread_pool_destroy(&pool);
  printf("skonczylem destroy\n");
}