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
static struct i2c_client *lcd_client = NULL;

int work_loop(void *dat) {
  static char PR1[17] = {0};
  static char PR2[17] = {0};

  while (!kthread_should_stop()) {
    int i;
    char content1[17];
    char content2[17];
    lcd_send_display_control(lcd_client);

    get_display_line1(content1);
    if (strcmp(PR1, content1)) {
      int last_c;
      strcpy(PR1, content1);
      lcd_set_cursor_position(lcd_client, 0, 0);
      for (i = 0; i < strlen(content1); i++) {
        lcd_write(lcd_client, content1[i]);
      }
      last_c = i;
      for (; i < 16; i++) {
        lcd_write(lcd_client, ' ');
      }
      lcd_set_cursor_position(lcd_client, last_c, 0);
    }

    get_display_line2(content2);
    if (strcmp(PR2, content2)) {
      int last_c;
      strcpy(PR2, content2);
      lcd_set_cursor_position(lcd_client, 0, 1);
      for (i = 0; i < strlen(content2); i++) {
        lcd_write(lcd_client, content2[i]);
      }
      last_c = i;
      for (; i < 16; i++) {
        lcd_write(lcd_client, ' ');
      }
      lcd_set_cursor_position(lcd_client, last_c, 1);
    }

    refresh();
    msleep(refresh_timeout);
  }
  do_exit(0);
  return 0;
}

static int lcd_probe(struct i2c_client *client) {
  pr_info("Probed %x", client->addr);

  if (client->addr == 0x27) {
    lcd_client = client;
    lcd_init(lcd_client);
    lcd_sysfs_init();
    task_struct = kthread_run(work_loop, NULL, "pcf lcd work loop");
    set_content("LCD\n Hello!");
    return 0;
  } else {
    pr_info("Don't care %d address", client->addr);
    return -1;
  }
  return 0;
}
static int lcd_remove(struct i2c_client *client) {
  set_content("LCD\n Goodbye!");
  refresh();
  msleep(refresh_timeout * 2);
  kthread_stop(task_struct);
  lcd_sysfs_deinit();

  pr_info("Removed\n");
  return 0;
}
static const struct of_device_id my_driver_ids[] = {{
                                                        .compatible = "pcf,lcd",
                                                    },
                                                    {/*sentinel*/}};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static const struct i2c_device_id lcd_device_id[] = {
    {DEVICE_NAME, 0},
    {},
};

MODULE_DEVICE_TABLE(i2c, lcd_device_id);

static struct i2c_driver lcd_driver = {
    .probe_new = lcd_probe,
    .remove = lcd_remove,
    .id_table = lcd_device_id,
    .driver = {.name = DEVICE_NAME,
               .owner = THIS_MODULE,
               .of_match_table = my_driver_ids},
};

module_i2c_driver(lcd_driver);

MODULE_LICENSE("GPL");
