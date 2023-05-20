#include "linux/moduleparam.h"
#include <linux/module.h>

int min_val(int arr[], int len) {
  int i, ret = arr[0];
  for (i = 1; i < len; i++) {
    if (arr[i] < ret) {
      ret = arr[i];
    }
  }
  return ret;
}
int max_val(int arr[], int len) {
  int i, ret = arr[0];
  for (i = 1; i < len; i++) {
    if (arr[i] > ret) {
      ret = arr[i];
    }
  }
  return ret;
}
int sum(int arr[], int len) {
  int i, ret = 0;
  for (i = 0; i < len; i++) {
    ret += arr[i];
  }
  return ret;
}

int init_module(void) {
  int arr[] = {1, 2, 3, 4};
  int len = 4;

  pr_info("Init module\n");
  pr_info("min = %d\n", min_val(arr, len));
  pr_info("max = %d\n", max_val(arr, len));
  pr_info("avg = %d\n", sum(arr, len));
  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
