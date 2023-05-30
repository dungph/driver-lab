#include "lcd_data.h"
#include "lcd_i2c_client.h"
#include "lcd_sysfs.h"
#include "linux/kernel.h"

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/stddef.h>
#include <linux/string.h>
#include <linux/types.h>

uint refresh_timeout = 1000;
static struct task_struct *task_struct;

int work_loop(void *dat) {
  static char PR1[17] = {0};
  static char PR2[17] = {0};

  while (!kthread_should_stop()) {
    msleep(refresh_timeout);
    lcd_send_display_control();
    pr_info("in loop\n");
    if (is_initialized()) {
      int i;
      char content1[17];
      char content2[17];
      get_display_line1(content1);
      get_display_line2(content2);
      pr_info("printing: \n%s\n%s", content1, content2);
      if (strcmp(PR1, content1) | strcmp(PR2, content2)) {
        lcd_clear();
        strcpy(PR1, content1);
        strcpy(PR2, content2);
        lcd_set_cursor_position(0, 0);
        for (i = 0; i < strlen(content1); i++) {
          lcd_write(content1[i]);
        }
        lcd_set_cursor_position(0, 1);
        for (i = 0; i < strlen(content2); i++) {
          lcd_write(content2[i]);
        }
      }
      refresh();
    }
  }
  do_exit(0);
  return 0;
}

int init_module(void) {
  pr_info("hello\n");
  i2c_client_init();
  lcd_sysfs_init();
  task_struct = kthread_run(work_loop, NULL, "i2c1602 work loop");
  set_content("LCD\n Hello!");
  return 0;
}

void cleanup_module(void) {
  set_content("LCD\n Goodbye!");
  refresh();
  kthread_stop(task_struct);
  lcd_sysfs_deinit();
  i2c_client_deinit();
  pr_info("goodbye\n");
  return;
}

MODULE_LICENSE("GPL");
