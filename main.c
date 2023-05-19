#include "vdso/limits.h"
#include <linux/module.h>
int sum(int arr[], int arr_len) {
  int sum = 0;
  int i;
  for (i = 0; i < arr_len; i++) {
    sum += arr[i];
  }
  return sum;
}

int init_module(void)

{
  int matrix[5][5];
  int n = 5;
  int i;
  int neg = 0, pos = 0;
  int odd_sum = 0, even_sum = 0;
  int min1 = INT_MAX, max1 = INT_MIN;
  int min2 = INT_MAX, max2 = INT_MIN;

  for (i = 0; i < n * n; i++) {
    matrix[i / n][i % n] = i + 1;
  }

  pr_info("Init module\n");

  pr_info("sum = %d\n", sum((int *)matrix, n * n));

  for (i = 0; i < n * n; i++) {
    if (matrix[i / n][i % n] > 0) {
      pos += 1;
    }
    if (matrix[i / n][i % n] < 0) {
      neg += 1;
    }
  }
  pr_info("number of positive value = %d\n", pos);
  pr_info("number of negative value = %d\n", neg);

  for (i = 0; i < n; i++) {
    if (matrix[i][i] > max1) {
      max1 = matrix[i][i];
    }
    if (matrix[i][i] < min1) {
      min1 = matrix[i][i];
    }
    if (matrix[i][n - i - 1] > max2) {
      max2 = matrix[i][n - i - 1];
    }
    if (matrix[i][n - i - 1] < min2) {
      min2 = matrix[i][n - i - 1];
    }
  }

  pr_info("min value in main diagonal is %d\n", min1);
  pr_info("max value in main diagonal is %d\n", max1);

  pr_info("min value in auxiliary diagonal is %d\n", min2);
  pr_info("max value in auxiliary diagonal is %d\n", max2);

  for (i = 0; i < n * n; i++) {
    if (matrix[i / n][i % n] % 2) {
      odd_sum += matrix[i / n][i % n];
    } else {
      even_sum += matrix[i / n][i % n];
    }
  }
  pr_info("sum of odd value is %d\n", odd_sum);
  pr_info("sum of even value is %d\n", even_sum);

  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
