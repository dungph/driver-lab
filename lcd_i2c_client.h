#ifndef LCD_I2C_CLIENT_H
#define LCD_I2C_CLIENT_H = 1

#include "lcd_data.h"
#include "linux/delay.h"
#include "linux/i2c.h"
#include "linux/module.h"
#include "linux/stddef.h"

#define DEVICE_NAME "pcf_lcd"

enum Backlight { BacklightOn = 0x08, BacklightOff = 0x00 };
enum Cursor { CursorOn = 0x02, CursorOff = 0x00 };
enum Blink { BlinkOn = 0x01, BlinkOff = 0x00 };
enum Display { DisplayOn = 0x04, DisplayOff = 0x00 };
enum BitAction {
  BACommand = 0x00,
  BAEnable = 0x04,
  BAReadWrite = 0x02,
  BARegisterSelect = 0x01
};

enum Mode {
  COMMAND = 0x00,
  CLEARDISPLAY = 0x01,
  RETURNHOME = 0x02,
  ENTRYMODESET = 0x04,
  DISPLAYCONTROL = 0x08,
  CURSORSHIFT = 0x10,
  FUNCTIONSET = 0x20,
  SETCGRAMADDR = 0x40,
  SETDDRAMADDR = 0x80,
};

enum BitMode {
  BitMode4 = 0x00,
  BitMode8 = 0x10,
};

enum Dots {
  Dots5x8 = 0x00,
  Dots5x10 = 0x04,
};

enum Lines {
  OneLine = 0x00,
  TwoLine = 0x08,
};
enum Entries {
  RIGHT = 0x00,
  LEFT = 0x02,
};
enum Shift {
  INCREMENT = 0x01,
  DECREMENT = 0x00,
};

static void lcd_write_byte(struct i2c_client *lcd_client, uint8_t byte) {
  static uint8_t sending = 0;
  sending = byte | (get_backlight() ? BacklightOn : BacklightOff);
  i2c_smbus_write_byte(lcd_client, sending);
  return;
}
static void lcd_pulse_enable(struct i2c_client *lcd_client, uint8_t byte) {
  lcd_write_byte(lcd_client, byte | BAEnable);
  usleep_range(500, 550);

  lcd_write_byte(lcd_client, byte & !(BAEnable));
  usleep_range(500, 550);
}

static int lcd_write4bits(struct i2c_client *lcd_client, uint8_t byte) {
  lcd_write_byte(lcd_client, byte);
  lcd_pulse_enable(lcd_client, byte);
  return 0;
}

static void lcd_send(struct i2c_client *lcd_client, uint8_t data,
                     enum BitAction mode) {
  uint8_t high_bits = data & 0xf0;
  uint8_t low_bits = (data << 4) & 0xf0;
  lcd_write4bits(lcd_client, high_bits | mode);
  lcd_write4bits(lcd_client, low_bits | mode);
}
static void lcd_command(struct i2c_client *lcd_client, uint8_t value) {
  lcd_send(lcd_client, value, BACommand);
  return;
}

static void lcd_write(struct i2c_client *lcd_client, uint8_t value) {
  lcd_send(lcd_client, value, BARegisterSelect);
  return;
}

void lcd_clear(struct i2c_client *lcd_client) {
  lcd_command(lcd_client, CLEARDISPLAY);
  usleep_range(500, 2000);
}

void lcd_send_display_control(struct i2c_client *lcd_client) {
  lcd_command(lcd_client, DISPLAYCONTROL |
                              ((get_cursor() ? CursorOn : CursorOff) |
                               (get_blink() ? BlinkOn : BlinkOff) |
                               (get_display() ? DisplayOn : DisplayOff) |
                               (get_backlight() ? BacklightOn : BacklightOff)));
}
void lcd_set_cursor_position(struct i2c_client *lcd_client, uint8_t col,
                             uint8_t row) {
  lcd_command(lcd_client, SETDDRAMADDR | (col + row * 0x40));
  return;
}
void lcd_write_at(struct i2c_client *lcd_client, uint8_t col, uint8_t row,
                  char c) {
  lcd_set_cursor_position(lcd_client, col, row);
  lcd_write(lcd_client, c);
}
void lcd_home(struct i2c_client *lcd_client) {
  lcd_command(lcd_client, RETURNHOME);
  return;
}

void lcd_init(struct i2c_client *lcd_client) {
  pr_info("lcd init\n");
  lcd_command(lcd_client, 0x03);
  lcd_command(lcd_client, 0x03);
  lcd_command(lcd_client, 0x03);
  lcd_command(lcd_client, 0x02);
  lcd_command(lcd_client, FUNCTIONSET | TwoLine | Dots5x8 | BitMode4);
  lcd_command(lcd_client, DISPLAYCONTROL | DisplayOn);
  lcd_command(lcd_client, CLEARDISPLAY);
  lcd_command(lcd_client, ENTRYMODESET | LEFT);
  usleep_range(200000, 300000);
}
#endif
