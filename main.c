#include "linux/export.h"
#include "linux/printk.h"
#include "linux/stddef.h"
#include "vchar_ioctl_cmd.h"
#include <linux/cdev.h>
#include <linux/crypto.h>
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

void caesar_encrypt(int key) {
  int i;
  for (i = 0; i < vchar_dr.buflen; i++) {
    char c = vchar_dr.buffer[i];
    if ('0' <= c && c <= '9') {
      c += key % 10;
      if (c > '9') {
        c -= 10;
      }
    }
    if ('a' <= c && c <= 'z') {
      c += key % 26;
      if (c > 'z') {
        c -= 26;
      }
    }
    if ('A' <= c && c <= 'Z') {
      c += key % 26;
      if (c > 'Z') {
        c -= 26;
      }
    }
    vchar_dr.buffer[i] = c;
  }
}
void caesar_decrypt(int key) {
  int i;
  for (i = 0; i < vchar_dr.buflen; i++) {
    char c = vchar_dr.buffer[i];
    if ('0' <= c && c <= '9') {
      c -= key % 10;
      if (c < '0') {
        c += 10;
      }
    }
    if ('a' <= c && c <= 'z') {
      c -= key % 26;
      if (c < 'a') {
        c += 26;
      }
    }
    if ('A' <= c && c <= 'Z') {
      c -= key % 26;
      if (c < 'A') {
        c += 26;
      }
    }
    vchar_dr.buffer[i] = c;
  }
}
void replace_encrypt(char *key) {
  int i;
  for (i = 0; i < vchar_dr.buflen; i++) {
    char c = vchar_dr.buffer[i];
    if ('a' <= c && c <= 'z') {
      c = key[c - 'a'];
    }
    vchar_dr.buffer[i] = c;
  }
}
void replace_decrypt(char *key) {
  int i;
  for (i = 0; i < vchar_dr.buflen; i++) {
    char c = vchar_dr.buffer[i];
    int j;
    for (j = 0; j < 26; j++) {
      if (key[j] == c) {
        c = j + 'a';
        break;
      }
    }
    vchar_dr.buffer[i] = c;
  }
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

  char buf[BUFFER_SIZE];

  switch (cmd) {
  case IOCTL_ENCRYPT_CAESAR:
    caesar_encrypt(arg);
    break;
  case IOCTL_DECRYPT_CAESAR:
    caesar_decrypt(arg);
    break;
  case IOCTL_ENCRYPT_REPLACE:
    copy_from_user(buf, (char __user *)arg, 27);
    replace_encrypt(buf);
    break;
  case IOCTL_DECRYPT_REPLACE:
    copy_from_user(buf, (char __user *)arg, 27);
    replace_decrypt(buf);
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
