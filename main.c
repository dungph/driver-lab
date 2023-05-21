#include "linux/export.h"
#include "linux/stddef.h"
#include "vchar_ioctl_cmd.h"
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

int my_dec_to_hex(char *dec, char *hex) {
  int ndec = 0;
  int len = strlen(dec);
  int i;

  for (i = len - 1; i >= 0; i--) {
    ndec *= 10;
    ndec += dec[i] - '0';
  }

  sprintf(hex, "%x", ndec);
  return 1;
}
int my_dec_to_oct(char *dec, char *hex) {
  int ndec = 0;
  int len = strlen(dec);
  int i;

  for (i = len - 1; i >= 0; i--) {
    ndec *= 10;
    ndec += dec[i] - '0';
  }

  sprintf(hex, "%o", ndec);
  return 1;
}
int my_dec_to_bin(char *dec, char *bin) {
  int ndec = 0;
  int len = strlen(dec);
  int i;
  int bin_len = 0;

  for (i = len - 1; i >= 0; i--) {
    ndec *= 10;
    ndec += dec[i] - '0';
  }
  while (ndec) {
    bin[bin_len] = ndec % 2 + '0';
    ndec /= 2;
    bin_len += 1;
  }
  bin[bin_len] = 0;
  for (i = 0; i < bin_len / 2; i++) {
    char t = bin[i];
    bin[i] = bin[bin_len - i - 1];
    bin[bin_len - i - 1] = t;
  }
  return 1;
}

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
static long vchar_dr_ioctl(struct file *file, unsigned int cmd,
                           unsigned long arg) {
  int ret = 0;
  int remains;

  char buf[BUFFER_SIZE];
  switch (cmd) {
  /* TODO 6/3: if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
  case IOCTL_INPUT:
    remains = copy_from_user(vchar_dr.buffer, (char __user *)arg, BUFFER_SIZE);
    if (remains)
      ret = -EFAULT;
    vchar_dr.buflen = BUFFER_SIZE - remains;
    break;
  /* TODO 7/19: extra tasks, for home */
  case IOCTL_READ_BIN:
    vchar_dr.buffer[vchar_dr.buflen] = 0;
    my_dec_to_bin(vchar_dr.buffer, buf);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  case IOCTL_READ_OCT:
    vchar_dr.buffer[vchar_dr.buflen] = 0;
    my_dec_to_oct(vchar_dr.buffer, buf);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  case IOCTL_READ_HEX:
    vchar_dr.buffer[vchar_dr.buflen] = 0;
    my_dec_to_hex(vchar_dr.buffer, buf);
    copy_to_user((char __user *)arg, buf, BUFFER_SIZE);
    break;
  default:
    ret = -EINVAL;
  }

  return ret;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = vchar_dr_open,
    .release = vchar_dr_release,
    .read = vchar_dr_read,
    .write = vchar_dr_write,
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
