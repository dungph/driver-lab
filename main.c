#include "linux/export.h"
#include "linux/stddef.h"
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
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

static ssize_t vchar_dr_read(struct file *file, char __user *user_buffer,
                             size_t size, loff_t *offset) {
  size_t to_read;
  pr_info("Reading from vchar_dr\n");
  to_read =
      (size > vchar_dr.buflen - *offset) ? (vchar_dr.buflen - *offset) : size;
  if (copy_to_user(user_buffer, vchar_dr.buffer + *offset, to_read) != 0)
    return -EFAULT;
  *offset += to_read;

  return to_read;
}

static ssize_t vchar_dr_write(struct file *file, const char __user *user_buffer,
                              size_t size, loff_t *offset) {

  pr_info("Writing to vchar_dr\n");
  size = (*offset + size > 1024) ? (1024 - *offset) : size;
  if (copy_from_user(vchar_dr.buffer + *offset, user_buffer, size) != 0)
    return -EFAULT;
  *offset += size;
  vchar_dr.buflen = *offset;

  return size;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = vchar_dr_open,
    .release = vchar_dr_release,
    .read = vchar_dr_read,
    .write = vchar_dr_write,
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
