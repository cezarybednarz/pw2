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

int defer_queue_push(defer_queue_t *q) {
  node_t *node = malloc(sizeof(node));
  if(node == NULL) {
    return -1;
  }

  if(q->length == 0) {
    // tutaj skonczylem
  }
  else {

  }
  return 0;
}

runnable_t *defer_queue_pop(defer_queue_t *q) {

}

int defer_queue_destroy(defer_queue_t *q) {

  return 0;
}
