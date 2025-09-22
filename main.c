#include <stdio.h>
#include <unistd.h>
#include "event_loop.h"

event_loop_t el;

int sum(int *nums, int size) {
  int res;
  int i;

  res = 0;
  for (i = 0; i < size; i++) {
    res += nums[i];
  }
  return res;
}

int mul(int *nums, int size) {
  int res;
  int i;

  res = 1;
  for (i = 0; i < size; i++) {
    res += nums[i];
  }
  return res;
}

typedef struct arg_obj_ {
  int *nums;
  int size;
} arg_obj_t;

void sum_fn(void *arg) {
  arg_obj_t *arg_obj;

  arg_obj = (arg_obj_t *) arg;
  printf("sum=%d\n", sum(arg_obj->nums, arg_obj->size));
}

void mul_fn(void *arg) {
  arg_obj_t *arg_obj;

  arg_obj = (arg_obj_t *) arg;
  printf("mul=%d\n", mul(arg_obj->nums, arg_obj->size));

}

int download = 0;
int upload = 0;

void download_fn(void *arg) {
  (void) arg;

  while (download < 100) {
    download += 2;
    printf("download: %d\n", download);
    if (download % 10 == 0) {
      event_loop_add_task(&el, download_fn, NULL);
      return;
    }
  }
  printf("download success\n");
}

void upload_fn(void *arg) {
  (void) arg;

  while (upload < 100) {
    upload += 2;
    printf("upload: %d\n", download);
    if (upload % 10 == 0) {
      event_loop_add_task(&el, upload_fn, NULL);
      return;
    }
  }
  printf("upload success\n");
}
arg_obj_t arg;
int nums[] = {1, 2, 3, 4, 5};

int main(void) {
  arg.nums = nums;
  arg.size = sizeof(nums) / sizeof(nums[0]);
  event_loop_init(&el);
  event_loop_run(&el);
  sleep(1);
  event_loop_add_task(&el, sum_fn, &arg);
  event_loop_add_task(&el, mul_fn, &arg);
  sleep(1);
  event_loop_add_task(&el, download_fn, NULL);
  event_loop_add_task(&el, upload_fn, NULL);
  printf("main done\n");
  pthread_exit(0);
}
