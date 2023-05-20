#include <linux/module.h>

int my_strlen(char *str) {
  int ret;
  for (ret = 0; str[ret] != 0; ret += 1) {
  }
  return ret;
}

void my_strcat(char *target, char *source) {
  int len = my_strlen(target);
  do {
    target[len] = *source;
    source++;
    len++;
  } while (*source != 0);
}

void trim_str(char *str) {
  char *src = str;
  char *dest = str;
  while (*src != 0) {
    while (*src != 0 && *src == ' ') {
      src++;
    }
    while (*src != 0 && *src != ' ') {
      *dest = *src;
      src++;
      dest++;
    }
    *dest = ' ';
    dest++;
  }
  while (*dest == ' ' || *dest == 0) {
    *dest = 0;
    dest--;
  }
}

void rev_by_length(char *str, int len) {
  int i;
  for (i = 0; i < len / 2; i++) {
    char t = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = t;
  }
}
void rev_by_endian(char *str) {
  int len = my_strlen(str);
  rev_by_length(str, len);
}

int init_module(void) {
  char str1[100] = "hello world";
  char str2[100] = "     hello     world     ";

  pr_info("Init module\n");
  pr_info("string %s has length of %d", str1, my_strlen(str1));

  pr_info("string %s length 5 rev is", str1);
  rev_by_length(str1, 5);
  pr_info("%s", str1);

  pr_info("string %s cat %s is ", str1, str2);
  my_strcat(str1, str2);
  pr_info("%s", str1);

  pr_info("string \"%s\" trim is ", str2);
  trim_str(str2);
  pr_info("\"%s\"", str2);

  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
