#include "defer_queue.h"

/* ---- defer queue ---- */
defer_queue_t *new_defer_queue(void) {

  defer_queue_t *queue = (defer_queue_t*)malloc(sizeof(defer_queue_t));
  if(queue == NULL) {
    return NULL;
  }

  queue->front = NULL;
  queue->back = NULL;
  queue->length = 0;

  return queue;
}

void defer_queue_print(defer_queue_t *q) {
  node_t *node = q->back;
  printf("queue of size %d: \n", (int)q->length);
  printf("[%zu] ", node->runnable->argsz);
  for(int i = 0; i < q->length - 1; i++) {
    node = node->prev;
    printf("[%zu] ", node->runnable->argsz);
  }
}

int defer_queue_push(defer_queue_t *q, runnable_t *runnable) {
  node_t *node = (node_t*)malloc(sizeof(node_t));
  if(node == NULL) {
    return -1;
  }

  node->runnable = runnable;

  if(q->length == 0) {
    q->front = node;
    q->back = node;
  }
  else {
    q->front->prev = node;
    q->front = node;
  }

  q->length++;

  return 0;
}

runnable_t *defer_queue_pop(defer_queue_t *q) {
  runnable_t *ret = q->back->runnable;

  node_t *back = q->back;

  if(q->length > 1) {
    q->back = q->back->prev;
  }

  free(back);

  q->length--;
  return ret;
}

void defer_queue_destroy(defer_queue_t *q) {
  while(q->length > 0) {
    defer_queue_pop(q);
  }
  free(q);
}
