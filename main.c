#include "linux/moduleparam.h"
#include <linux/module.h>

int sum(int arr[], int len) {
  int i, ret = 0;
  for (i = 0; i < len; i++) {
    ret += arr[i];
  }
  return ret;
}

int gcd(int first_num, int second_num) {
  while (first_num != second_num) {
    if (first_num > second_num) {
      first_num -= second_num;
    } else {
      second_num -= first_num;
    }
  }
  return first_num;
}
int init_module(void) {
  int arr[] = {1, 2, 3, 4};
  int len = 4;
  int n1 = 12;
  int n2 = 16;

  pr_info("Init module\n");

  pr_info("sum = %d\n", sum(arr, len));
  pr_info("gcd = %d\n", gcd(n1, n2));
  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
