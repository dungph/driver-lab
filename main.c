#include "linux/moduleparam.h"
#include <linux/module.h>

int arr[100] = {1, 2};
int count;

module_param_array(arr, int, &count, 0);
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
  int i;
  int prev = arr[0];

  pr_info("Init module\n");

  for (i = 1; i < count; i++) {
    prev = gcd(prev, arr[i]);
  }
  pr_info("gcd = %d\n", prev);
  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
