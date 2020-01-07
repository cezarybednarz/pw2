#include <unistd.h>

#include "threadpool.h"


static void wypiszA(void *arg __attribute__((unused)), size_t argsz __attribute__((unused))) {
  printf("beginning task A\n");
  for(int i = 0; i < 4; i++) {
    sleep(1);
    printf("A\n");
  }
  printf("ending task A\n");
}

static void wypiszB(void *arg __attribute__((unused)), size_t argsz __attribute__((unused))) {
  printf("beginning task B\n");
  for(int i = 0; i < 5; i++) {
    sleep(1);
    printf("B\n");
  }
  printf("ending task B\n");
}

int main() {

  thread_pool_t *pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));
  thread_pool_init(pool, 2);

  for(int i = 0; i < 1; i++) {
    defer(pool, (runnable_t) {.function = wypiszA, .arg = NULL, .argsz = 420});
    defer(pool, (runnable_t) {.function = wypiszB, .arg = NULL, .argsz = 69});
  }

  sleep(10000);

  printf("zaczynam destroy\n");
  thread_pool_destroy(pool);
  printf("skonczylem destroy\n");
  free(pool);
}