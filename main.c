#include "linux/export.h"
#include "linux/stddef.h"
#include "vchar_ioctl_cmd.h"
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/ktime.h>
#include <linux/module.h>

struct _vchar_dr {
  dev_t dev_num;
  struct class *dev_class;
  struct device *dev;
  struct cdev *vcdev;
  char buffer[1024];
  int buflen;
} vchar_dr;

static int vchar_dr_open(struct inode *inode, struct file *file) {
  pr_info("driver open\n");
  return 0;
}
static int vchar_dr_release(struct inode *inode, struct file *file) {
  pr_info("driver release\n");
  return 0;
}

static long vchar_dr_ioctl(struct file *file, unsigned int cmd,
                           unsigned long arg) {
  int ret = 0;
  int remains;

  char buf[BUFFER_SIZE];
  uint64_t time;
  ktime_t ktime;
  switch (cmd) {
    /* TODO 6/3: if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
    /* TODO 7/19: extra tasks, for home */
  case IOCTL_GET_MICRO_S:
    time = ktime_get_ns();
    sprintf(buf, "%lld", time / 1000);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  case IOCTL_GET_NANO_S:
    time = ktime_get_ns();
    sprintf(buf, "%lld", time);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  case IOCTL_GET_TIME:
    ktime = ktime_get_real();
    sprintf(buf, "%lld", ktime);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  // case IOCTL_READ_BIN:
  //   vchar_dr.buffer[vchar_dr.buflen] = 0;
  //   copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
  //   break;
  // case IOCTL_READ_OCT:
  //   vchar_dr.buffer[vchar_dr.buflen] = 0;
  //   my_dec_to_oct(vchar_dr.buffer, buf);
  //   copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
  //   break;
  // case IOCTL_READ_HEX:
  //   vchar_dr.buffer[vchar_dr.buflen] = 0;
  //   my_dec_to_hex(vchar_dr.buffer, buf);
  //   copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
  //   break;
  default:
    ret = -EINVAL;
  }

  return ret;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = vchar_dr_open,
    .release = vchar_dr_release,
    .unlocked_ioctl = vchar_dr_ioctl,
};
int init_module(void) {
  pr_info("Init module\n");
  vchar_dr.dev_num = 0;
  alloc_chrdev_region(&vchar_dr.dev_num, 0, 1, "vchar_device");
  vchar_dr.dev_class = class_create(THIS_MODULE, "class_vchar_dev");
  vchar_dr.dev = device_create(vchar_dr.dev_class, NULL, vchar_dr.dev_num, NULL,
                               "vchar_dev");
  vchar_dr.vcdev = cdev_alloc();
  cdev_init(vchar_dr.vcdev, &fops);
  cdev_add(vchar_dr.vcdev, vchar_dr.dev_num, 1);
  strcpy(vchar_dr.buffer, "Hello world\n");
  vchar_dr.buflen = 12;
  return 0;
}

void cleanup_module(void) {
  pr_info("Cleanup module\n");
  cdev_del(vchar_dr.vcdev);
  device_destroy(vchar_dr.dev_class, vchar_dr.dev_num);
  class_destroy(vchar_dr.dev_class);
  unregister_chrdev_region(vchar_dr.dev_num, 2);
}

MODULE_LICENSE("GPL");
