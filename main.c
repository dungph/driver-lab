#include <linux/module.h>

int init_module(void)

{
  pr_info("Init module\n");
  return 0;
}

void cleanup_module(void)

{
  pr_info("Exit module\n");
}

MODULE_LICENSE("GPL");
