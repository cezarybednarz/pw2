#include "defer_queue.h"

/* ---- defer queue ---- */
defer_queue_t *new_defer_queue(void) {

  defer_queue_t *queue = malloc(sizeof(defer_queue_t));
  if(queue == NULL) {
    return NULL;
  }

  sem_init(&(queue->sem_not_empty), 0, 0);
  queue->front = NULL;
  queue->back = NULL;
  queue->length = 0;

  return 0;
}

int defer_queue_push(defer_queue_t *q) {

  return 0;
}

runnable_t *defer_queue_pop(defer_queue_t *q) {

}

int defer_queue_destroy(defer_queue_t *q) {

  return 0;
}
