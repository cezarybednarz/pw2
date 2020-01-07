#include <unistd.h>

#include "threadpool.h"

pthread_mutex_t matrix_mutex;
int *result;

typedef struct elem {
  int a;
  int time;
} elem_t;

typedef struct row {
  elem_t *row;
  int row_id;
} row_t;

static void sum_row(void *arg, size_t argsz) {
  row_t *curr = (row_t*)arg;
  int sum = 0;
  for(int i = 0; i < argsz; i++) {
    sum += curr->row[i].a;
  }
  pthread_mutex_lock(&matrix_mutex);
  result[curr->row_id] += sum;
  pthread_mutex_unlock(&matrix_mutex);
  free(arg);
}

int main() {

  thread_pool_t *pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));
  thread_pool_init(pool, 4);

  int n, m;
  scanf("%d%d", &n, &m);

  pthread_mutex_init(&matrix_mutex, NULL);
  result = malloc(n * sizeof(int));
  for(int i = 0; i < n; i++)
    result[i] = 0;

  elem_t **t = malloc(n * sizeof(elem_t*));

  for(int i = 0; i < n; i++) {
    t[i] = malloc(m * sizeof(elem_t));
    for(int j = 0; j < m; j++) {
      scanf("%d %d", &t[i][j].a, &t[i][j].time);
    }

    row_t *curr_row = malloc(sizeof(row_t));
    curr_row->row = t[i];
    curr_row->row_id = i;
    runnable_t runnable;
    runnable.function = sum_row;
    runnable.arg = (void*)curr_row;
    runnable.argsz = m;
    defer(pool, runnable);
  }

  thread_pool_destroy(pool);

  for(int i = 0; i < n; i++) {
    printf("%d\n", result[i]);
  }

  for(int i = 0; i < n; i++) {
    free(t[i]);
  }
  free(t);
  free(result);
  free(pool);
}