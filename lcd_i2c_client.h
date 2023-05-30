#ifndef LCD_I2C_CLIENT_H
#define LCD_I2C_CLIENT_H = 1

#include "lcd_data.h"
#include "linux/delay.h"
#include "linux/i2c.h"
#include "linux/module.h"
#include "linux/stddef.h"

#define DEVICE_NAME "i2c1602"

uint initialized = 0;
static struct i2c_client *lcd_client = NULL;
static struct i2c_adapter *i2c_adapter;

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

static void lcd_write_byte(uint8_t byte) {
  static uint8_t sending = 0;
  sending = byte | (get_backlight() ? BacklightOn : BacklightOff);
  i2c_smbus_write_byte(lcd_client, sending);
  return;
}
static void lcd_pulse_enable(uint8_t byte) {
  lcd_write_byte(byte | BAEnable);
  usleep_range(100, 1000);

  lcd_write_byte(byte & !(BAEnable));
  usleep_range(100, 1000);
}

static int lcd_write4bits(uint8_t byte) {
  lcd_write_byte(byte);
  lcd_pulse_enable(byte);
  return 0;
}

static void lcd_send(uint8_t data, enum BitAction mode) {
  uint8_t high_bits = data & 0xf0;
  uint8_t low_bits = (data << 4) & 0xf0;
  lcd_write4bits(high_bits | mode);
  lcd_write4bits(low_bits | mode);
}
static void lcd_command(uint8_t value) {
  lcd_send(value, BACommand);
  return;
}

static void lcd_write(uint8_t value) {
  pr_info("sending %c\n", value);
  lcd_send(value, BARegisterSelect);
  return;
}

void lcd_clear(void) {
  lcd_command(CLEARDISPLAY);
  usleep_range(500, 2000);
}

void lcd_send_display_control(void) {
  lcd_command(DISPLAYCONTROL |
              ((get_cursor() ? CursorOn : CursorOff) |
               (get_blink() ? BlinkOn : BlinkOff) |
               (get_display() ? DisplayOn : DisplayOff) |
               (get_backlight() ? BacklightOn : BacklightOff)));
}
void lcd_set_cursor_position(uint8_t col, uint8_t row) {
  lcd_command(SETDDRAMADDR | (col + row * 0x40));
  return;
}
void lcd_write_at(uint8_t col, uint8_t row, char c) {
  lcd_set_cursor_position(col, row);
  lcd_write(c);
}
void lcd_home(void) {
  lcd_command(RETURNHOME);
  return;
}

void lcd_init(void) {
  pr_info("lcd init");
  lcd_command(0x03);
  lcd_command(0x03);
  lcd_command(0x03);
  lcd_command(0x02);
  lcd_command(FUNCTIONSET | TwoLine | Dots5x8 | BitMode4);
  lcd_command(DISPLAYCONTROL | DisplayOn);
  lcd_command(CLEARDISPLAY);
  lcd_command(ENTRYMODESET | LEFT);
  usleep_range(200000, 300000);
  pr_info("init done\n");
}

static int lcd_probe(struct i2c_client *client) {
  pr_info("Probed %x", client->addr);

  if (client->addr == 0x27) {
    lcd_client = client;
    lcd_init();
    initialized = 1;
    return 0;
  } else {
    pr_info("Don't care %d address", client->addr);
    return -1;
  }
  return 0;
}
static int lcd_remove(struct i2c_client *client) {
  initialized = 0;
  pr_info("Removed\n");
  return 0;
}

static const struct i2c_device_id lcd_device_id[] = {
    {DEVICE_NAME, 0},
    {},
};

MODULE_DEVICE_TABLE(i2c, lcd_device_id);

static struct i2c_driver lcd_driver = {
    .probe_new = lcd_probe,
    .remove = lcd_remove,
    .id_table = lcd_device_id,
    .driver =
        {
            .name = DEVICE_NAME,
            .owner = THIS_MODULE,
        },
};

static struct i2c_board_info board_info = {
    I2C_BOARD_INFO(DEVICE_NAME, 0x27),
};

void i2c_client_init(void) {
  i2c_adapter = i2c_get_adapter(1);
  lcd_client = i2c_new_client_device(i2c_adapter, &board_info);
  i2c_add_driver(&lcd_driver);
  i2c_put_adapter(i2c_adapter);
}
void i2c_client_deinit(void) {
  i2c_unregister_device(lcd_client);
  i2c_del_driver(&lcd_driver);
}

uint is_initialized(void) { return initialized; }

#endif
