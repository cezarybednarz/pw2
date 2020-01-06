#include "defer_queue.h"

/* ---- defer queue ---- */
defer_queue_t *new_defer_queue(void) {

  defer_queue_t *queue = malloc(sizeof(defer_queue_t));
  if(queue == NULL) {
    return NULL;
  }

  queue->front = NULL;
  queue->back = NULL;
  queue->length = 0;

  return queue;
}

int defer_queue_push(defer_queue_t *q, runnable_t *runnable) {
  node_t *node = malloc(sizeof(node_t));
  if(node == NULL) {
    return -1;
  }

  node->runnable = runnable;

  if(q->length == 0) {
    q->front = node;
    q->back = node;
  }
  else {
    q->back->prev = node;
    q->back = node;
  }

  q->length++;

  return 0;
}

runnable_t *defer_queue_pop(defer_queue_t *q) {
  runnable_t *ret = q->front->runnable;

  if(q->length > 1) {
    q->front = q->front->prev;
  }

  free(q->front);
  q->length--;
  return ret;
}

void defer_queue_destroy(defer_queue_t *q) {
  while(q->length > 0) {
    defer_queue_pop(q);
  }
}
