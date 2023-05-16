#include "linux/export.h"
#include "linux/stddef.h"
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>

struct _vchar_dr {
  dev_t dev_num;
  struct class *dev_class;
  struct device *dev;
} vchar_dr;

int init_module(void) {
  int ret = 0;

  vchar_dr.dev_num = 0;
  ret = alloc_chrdev_region(&vchar_dr.dev_num, 0, 1, "vchar_device");

  if (ret < 0) {
    pr_info("failed to create device file\n");
    return ret;
  }
  pr_info("alloc device file success\n");

  vchar_dr.dev_class = class_create(THIS_MODULE, "class_vchar_dev");
  if (vchar_dr.dev_class) {
    pr_info("create device class success\n");
  } else {
    unregister_chrdev_region(vchar_dr.dev_num, 1);
  }

  vchar_dr.dev = device_create(vchar_dr.dev_class, NULL, vchar_dr.dev_num, NULL,
                               "vchar_dev");
  if (vchar_dr.dev) {
    pr_info("create device success\n");
  } else {
    pr_info("create device failed\n");
    class_destroy(vchar_dr.dev_class);
    unregister_chrdev_region(vchar_dr.dev_num, 1);
  }

  return 0;
}

void cleanup_module(void) {
  device_destroy(vchar_dr.dev_class, vchar_dr.dev_num);
  class_destroy(vchar_dr.dev_class);
  unregister_chrdev_region(vchar_dr.dev_num, 1);
  pr_info("Exit vchar mod\n");
}

MODULE_LICENSE("GPL");
