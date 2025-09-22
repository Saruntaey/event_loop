#ifndef __EVENT_LOOP__
#define __EVENT_LOOP__

#include <pthread.h>
#include "glthread.h"

typedef void (*fn_cb)(void *);

typedef struct task_ {
  fn_cb cb;
  void *arg;
  glthread_node_t node;
} task_t;

typedef struct event_loop_ {
  pthread_t thread;
  pthread_mutex_t mu;
  pthread_cond_t have_task;
  glthread_t glthread;
  task_t *current_task;
} event_loop_t;

void event_loop_init(event_loop_t *el);
void event_loop_run(event_loop_t *el);
task_t *event_loop_add_task(event_loop_t *el, fn_cb cb, void *arg);
void event_loop_cancel_task(event_loop_t *el, task_t *t);

#endif
