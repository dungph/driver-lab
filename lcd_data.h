#ifndef LCD_DATA_H
#define LCD_DATA_H = 1

#include "linux/printk.h"
#include "linux/types.h"
#define CONTENT_MAX_LEN 1000

char __content[CONTENT_MAX_LEN + 2] = {0};
char __display_lines[100][101];
uint __nline;
uint __l1_idx = -1;
uint __l2_idx = -1;
uint __l1_off = -1;
uint __l2_off = -1;
uint __backlight = 1;
uint __cursor = 1;
uint __blink = 1;
uint __display = 1;
uint __auto_newline = 1;
uint __scroll = 0;
uint __scroll_start = 0;
uint __l1_scroll_done = 0;
uint __l2_scroll_done = 0;

uint strlen(const char *data) {
  uint len = 0;
  for (len = 0; data[len] != 0; len++) {
  }
  return len;
}
uint nline(void) { return __nline; }
uint line_len(const char *data) {
  uint len = 0;
  for (len = 0; data[len] != '\n' && data[len] != 0; len++) {
  }
  return len;
}

void init_display_lines(void) {
  uint line = 0;
  char *content = __content;

  while (*content != 0) {
    uint llen = line_len(content);
    if (__auto_newline) {
      uint i;
      for (i = 0; i < llen; i++) {
        __display_lines[line + i / 16][i % 16] = content[i];
        if ((i + 1) % 16 == 0) {
          __display_lines[line + i / 16][16] = 0;
        }
        if ((i + 1) == llen) {
          __display_lines[line + i / 16][i % 16 + 1] = 0;
        }
      }

      line += i / 16 + 1;
    } else {
      uint i;
      for (i = 0; i < llen; i++) {
        __display_lines[line][i] = content[i];
      }
      __display_lines[line][i] = 0;
      line += 1;
    }

    content += llen;
    if (*content == '\n') {
      content += 1;
    }
  }
  __scroll_start = 0;
  __nline = line;
  __l1_idx = 0;
  __l1_off = 0;
  __l2_idx = 1;
  __l2_off = 0;
  if (__nline == 0) {
    __display_lines[0][0] = 0;
    __display_lines[1][0] = 0;
    __nline = 2;
  } else if (__nline == 1) {
    __display_lines[1][0] = 0;
    __nline = 2;
  }
  if (__nline % 2) {
    __display_lines[__nline][0] = 0;
    __nline += 1;
  }
}
uint set_content(const char *new_content) {
  int i;
  for (i = 0; i < CONTENT_MAX_LEN && new_content[i] != 0; i++) {
    __content[i] = new_content[i];
  }
  __content[i] = 0;
  init_display_lines();
  return i;
}
uint get_content(char *dest) {
  int i;
  for (i = 0; i < CONTENT_MAX_LEN && __content[i] != 0; i++) {
    dest[i] = __content[i];
  }
  dest[i] = 0;

  return i;
}

uint get_backlight(void) { return __backlight; }
void set_backlight(uint backlight) { __backlight = backlight; }
uint get_cursor(void) { return __cursor; }
void set_cursor(uint cursor) { __cursor = cursor; }
uint get_blink(void) { return __blink; }
void set_blink(uint blink) { __blink = blink; }
uint get_display(void) { return __display; }
void set_display(uint display) { __display = display; }
uint get_scroll(void) { return __scroll; }
void set_scroll(uint scroll) { __scroll = scroll; }
uint get_auto_newline(void) { return __auto_newline; }
void set_auto_newline(uint n) {
  __auto_newline = n;
  init_display_lines();
}

#define get_display_line1(dest)                                                \
  {                                                                            \
    int i;                                                                     \
    for (i = 0; i < 16 && __content[__l1_idx + i] != 0; i++) {                 \
      (dest)[i] = __display_lines[__l1_idx][__l1_off + i];                     \
    }                                                                          \
    (dest)[i] = 0;                                                             \
  }

#define get_display_line2(dest)                                                \
  {                                                                            \
    int i;                                                                     \
    for (i = 0; i < 16 && __content[__l2_idx + i] != 0; i++) {                 \
      (dest)[i] = __display_lines[__l2_idx][__l2_off + i];                     \
    }                                                                          \
    (dest)[i] = 0;                                                             \
  }

void inc_offset(void) {
  if (strlen(__display_lines[__l1_idx]) - __l1_off > 16) {
    __l1_off += 1;
  } else {
    __l1_off = 0;
    __l1_scroll_done = 1;
  }
  if (strlen(__display_lines[__l2_idx]) - __l2_off > 16) {
    __l2_off += 1;
  } else {
    __l2_off = 0;
    __l2_scroll_done = 1;
  }
}

void refresh(void) {
  if (get_scroll()) {
    if (__scroll_start) {
      if (__l1_scroll_done && __l2_scroll_done) {
        __l1_idx = (__l1_idx + 2) % __nline;
        __l2_idx = (__l2_idx + 2) % __nline;
        __l1_off = 0;
        __l2_off = 0;
        __l1_scroll_done = 0;
        __l2_scroll_done = 0;
      } else {
        inc_offset();
      }
    } else {
      __scroll_start = 1;
    }
  }
}

#endif
