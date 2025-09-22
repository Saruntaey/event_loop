#include "event_loop.h"
#include "glthread.h"
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

static void *event_loop_fn(void *arg) {
  event_loop_t *el;
  task_t *task;

  el = (event_loop_t *) arg;
  for (;;) {
    pthread_mutex_lock(&el->mu);
    el->current_task = NULL;
    while (el->glthread.size == 0) {
      pthread_cond_wait(&el->have_task, &el->mu);
    }
    task = tostruct(task_t, (&el->glthread), glthread_remove(&el->glthread, NULL));
    el->current_task = task;
    pthread_mutex_unlock(&el->mu);
    task->cb(task->arg);
    free(task);
    task = NULL;
  }
  return NULL;
}


void event_loop_init(event_loop_t *el) {
  el->thread = 0;
  pthread_mutex_init(&el->mu, NULL);
  pthread_cond_init(&el->have_task, NULL);
  glthread_init(&el->glthread, offsetof(task_t, node));
}

void event_loop_run(event_loop_t *el) {
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&el->thread, &attr, event_loop_fn, el);
}

task_t *event_loop_add_task(event_loop_t *el, fn_cb cb, void *arg) {
  task_t *t;

  t = malloc(sizeof(*t));
  t->cb = cb;
  t->arg = arg;
  pthread_mutex_lock(&el->mu);
  glthread_add(&el->glthread, &t->node);
  pthread_cond_signal(&el->have_task);
  pthread_mutex_unlock(&el->mu);
  return t;
}

void event_loop_cancel_task(event_loop_t *el, task_t *t) {
  pthread_mutex_lock(&el->mu);
  if (el->current_task == t) return;
  glthread_remove(&el->glthread, &t->node);
  pthread_mutex_unlock(&el->mu);
  free(t);
}
