#ifndef LCD_SYSFS_H
#define LCD_SYSFS_H = 1

#include "lcd_data.h"
#include "linux/kernel.h"
#include "linux/kobject.h"
#include "linux/sysfs.h"
#include "linux/types.h"

#define ATTR_RW(v) struct kobj_attribute v##_attr = __ATTR_RW(v)
#define ATTR_RO(v) struct kobj_attribute v##_attr = __ATTR_RO(v)
#define ATTR_WO(v) struct kobj_attribute v##_attr = __ATTR_WO(v)

#define DECL_ATTR_SHOW(f)                                                      \
  ssize_t f##_show(struct kobject *kobj, struct kobj_attribute *attr,          \
                   char *buffer)

#define DECL_ATTR_STORE(f)                                                     \
  ssize_t f##_store(struct kobject *kobj, struct kobj_attribute *attr,         \
                    const char *buffer, size_t count)

static struct kobject *lcd_kobj;

static DECL_ATTR_SHOW(content) {
  char content[CONTENT_MAX_LEN];
  get_content(content);
  return sprintf(buffer, "%s", content);
}
static DECL_ATTR_STORE(content) {
  set_content(buffer);
  return count;
}
static DECL_ATTR_SHOW(line1) {
  char content[17];
  get_display_line1(content);
  return sprintf(buffer, "%s", content);
}
static DECL_ATTR_SHOW(line2) {
  char content[17];
  get_display_line2(content);
  return sprintf(buffer, "%s", content);
}

static DECL_ATTR_SHOW(scroll) {
  {}
  return sprintf(buffer, "%d", __scroll);
}
static DECL_ATTR_STORE(scroll) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_scroll(n);
  return count;
}
static DECL_ATTR_SHOW(backlight) {
  {}
  return sprintf(buffer, "%d", __backlight);
}
static DECL_ATTR_STORE(backlight) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_backlight(n);
  return count;
}
static DECL_ATTR_SHOW(auto_newline) {
  {}
  return sprintf(buffer, "%d", __auto_newline);
}
static DECL_ATTR_STORE(auto_newline) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_auto_newline(n);
  return count;
}
static DECL_ATTR_SHOW(cursor) {
  {}
  return sprintf(buffer, "%d", __cursor);
}
static DECL_ATTR_STORE(cursor) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_cursor(n);
  return count;
}
static DECL_ATTR_SHOW(blink) {
  {}
  return sprintf(buffer, "%d", __blink);
}
static DECL_ATTR_STORE(blink) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_blink(n);
  return count;
}
static DECL_ATTR_SHOW(display) {
  {}
  return sprintf(buffer, "%d", __display);
}
static DECL_ATTR_STORE(display) {
  uint n;
  sscanf(buffer, "%d", &n);
  set_display(n);
  return count;
}

static ATTR_RO(line1);
static ATTR_RO(line2);
static ATTR_RW(scroll);
static ATTR_RW(backlight);
static ATTR_RW(auto_newline);
static ATTR_RW(cursor);
static ATTR_RW(blink);
static ATTR_RW(display);
static ATTR_RW(content);

static struct attribute *attrs[] = {
    &line1_attr.attr,
    &line2_attr.attr,
    &scroll_attr.attr,
    &backlight_attr.attr,
    &auto_newline_attr.attr,
    &cursor_attr.attr,
    &blink_attr.attr,
    &display_attr.attr,
    &content_attr.attr,
    NULL,
    NULL,
};
static struct attribute_group attr_group = {
    .attrs = attrs,
};

void lcd_sysfs_init(void) {
  lcd_kobj = kobject_create_and_add("pcf_lcd", kernel_kobj);
  if (sysfs_create_group(lcd_kobj, &attr_group)) {
    kobject_put(lcd_kobj);
  }
}
void lcd_sysfs_deinit(void) {
  sysfs_remove_group(lcd_kobj, &attr_group);
  kobject_put(lcd_kobj);
}
#endif
