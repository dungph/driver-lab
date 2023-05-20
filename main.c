#include <linux/module.h>

int dot_product(int *va, int *vb, int dem) {
  int ret = 0;
  int i;

  for (i = 0; i < dem; i++) {
    ret += va[i] * vb[i];
  }
  return ret;
}

void add_matrix(int *res, int *m1, int *m2, int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      res[i * m + j] = m1[i * m + j] + m2[i * m + j];
    }
  }
}
void sub_matrix(int *res, int *m1, int *m2, int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      res[i * m + j] = m1[i * m + j] - m2[i * m + j];
    }
  }
}
void mul_matrix(int *res, int *m1, int *m2, int n, int m, int p) {
  int i, j, k;
  for (i = 0; i < n; i++) {
    for (k = 0; k < p; k++) {
      res[i * m + k] = 0;
      for (j = 0; j < m; j++) {
        res[i * m + k] += m1[i * m + j] * m2[j * m + k];
      }
    }
  }
}
void pr_mx(int *mt, int n, int m, char *name) {
  int i, j;
  pr_info("matrix %s:\n", name);
  for (i = 0; i < n; i++) {
    char buf[1000] = {0};
    for (j = 0; j < m; j++) {
      char buf2[10];
      sprintf(buf2, "%3d ", mt[i * m + j]);
      strcat(buf, buf2);
    }
    pr_info("%s\n", buf);
  }
}

int init_module(void) {
  pr_info("Init module\n");
  {
    int vec1[4] = {1, 2, 3, 4};
    int vec2[4] = {1, 2, 3, 4};
    pr_mx((int *)vec1, 1, 4, "vec1");
    pr_mx((int *)vec2, 1, 4, "vec2");
    pr_info("vec1.vec2=%d", dot_product(vec1, vec2, 4));
  }
  {
    int m1[4][4];
    int m2[4][4];
    int mres[4][4];
    int n = 4;
    int i;

    for (i = 0; i < n * n; i++) {
      m1[i / n][i % n] = i + 1;
      m2[i / n][i % n] = i + 1;
    }
    pr_mx((int *)m1, 4, 4, "m1");
    pr_mx((int *)m2, 4, 4, "m2");
    pr_info("m1 + m2:\n");
    add_matrix((int *)mres, (int *)m1, (int *)m2, n, n);
    pr_mx((int *)mres, 4, 4, "mres");

    pr_info("m1 - m2:\n");
    sub_matrix((int *)mres, (int *)m1, (int *)m2, n, n);
    pr_mx((int *)mres, 4, 4, "mres");

    pr_info("m1 * m2:\n");
    mul_matrix((int *)mres, (int *)m1, (int *)m2, n, n, n);
    pr_mx((int *)mres, 4, 4, "mres");
  }
  {
    int n = 20;
    int k = 10;
    long long res = 1;
    int i;
    for (i = n - k; i <= n; i++) {
      res *= i;
    }
    pr_info("A(%d, %d) = %lld", n, k, res);
  }
  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
