#include "asm/string_64.h"
#include "linux/kernel.h"
#include <linux/module.h>

int my_hex_to_bin(char *hex, char *bin) {
  int len = strlen(hex);
  int i;

  bin[0] = 0;

  for (i = 0; i < len; i++) {
    switch (hex[i]) {
    case '0':
      strcat(bin, "0000");
      break;
    case '1':
      strcat(bin, "0001");
      break;
    case '2':
      strcat(bin, "0010");
      break;
    case '3':
      strcat(bin, "0011");
      break;
    case '4':
      strcat(bin, "0100");
      break;
    case '5':
      strcat(bin, "0101");
      break;
    case '6':
      strcat(bin, "0110");
      break;
    case '7':
      strcat(bin, "0111");
      break;
    case '8':
      strcat(bin, "1000");
      break;
    case '9':
      strcat(bin, "1001");
      break;
    case 'a':
      strcat(bin, "1010");
      break;
    case 'b':
      strcat(bin, "1011");
      break;
    case 'c':
      strcat(bin, "1100");
      break;
    case 'd':
      strcat(bin, "1101");
      break;
    case 'e':
      strcat(bin, "1110");
      break;
    case 'f':
      strcat(bin, "1111");
      break;
    case 'A':
      strcat(bin, "1010");
      break;
    case 'B':
      strcat(bin, "1011");
      break;
    case 'C':
      strcat(bin, "1100");
      break;
    case 'D':
      strcat(bin, "1101");
      break;
    case 'E':
      strcat(bin, "1110");
      break;
    case 'F':
      strcat(bin, "1111");
      break;
    case ' ':
      break;
    default:
      return 0;
    }
  }
  return 1;
}
int my_dec_to_hex(char *dec, char *hex) {
  int ndec = 0;
  int len = strlen(dec);
  int i;

  for (i = 0; i < len; i++) {
    ndec *= 10;
    ndec += dec[i] - '0';
  }

  sprintf(hex, "%x", ndec);
  return 1;
}
int my_dec_to_bin(char *dec, char *bin) {
  char hex[100];
  my_dec_to_hex(dec, hex);
  my_hex_to_bin(hex, bin);
  return 1;
}

int my_bin_to_dec(char *bin, char *dec) {
  int ndec = 0;
  int len = strlen(bin);
  int i;
  int count = 0;
  for (i = len - 1; i >= 0; i--) {
    ndec += (bin[i] - '0') << count;
    count += 1;
  }
  sprintf(dec, "%d", ndec);
  return 1;
}

int my_bin_to_oct(char *bin, char *oct) {
  int dec = 0;
  int len = strlen(bin);
  int i;
  int count = 0;
  for (i = len - 1; i >= 0; i--) {
    dec += (bin[i] - '0') << count;
    count += 1;
  }
  sprintf(oct, "%o", dec);
  return 1;
}

int my_oct_to_bin(char *oct, char *bin) {
  int len = strlen(oct);
  int i;

  bin[0] = 0;

  for (i = 0; i < len; i++) {
    switch (oct[i]) {
    case '0':
      strcat(bin, "000");
      break;
    case '1':
      strcat(bin, "001");
      break;
    case '2':
      strcat(bin, "010");
      break;
    case '3':
      strcat(bin, "011");
      break;
    case '4':
      strcat(bin, "100");
      break;
    case '5':
      strcat(bin, "101");
      break;
    case '6':
      strcat(bin, "110");
      break;
    case '7':
      strcat(bin, "111");
      break;
    case ' ':
      break;
    default:
      return 0;
    }
  }
  return 1;
}
int my_oct_to_hex(char *oct, char *hex) {
  char bin[100];
  char dec[100];
  my_oct_to_bin(oct, bin);
  my_bin_to_dec(bin, dec);
  my_dec_to_hex(dec, hex);
  return 1;
}
int my_hex_to_oct(char *hex, char *oct) {
  char bin[100];
  my_hex_to_bin(hex, bin);
  my_bin_to_oct(bin, oct);
  return 1;
}

int init_module(void) {
  char result[100];
  char hex[] = "1B24";
  char dec[] = "1234";
  char oct[] = "1657";
  char bin[] = "10001001";

  pr_info("Init module\n");

  my_hex_to_bin(hex, result);
  pr_info("0x%s to bin = %s", hex, result);

  my_dec_to_bin(dec, result);
  pr_info("%s to bin = %s", dec, result);

  my_dec_to_hex(dec, result);
  pr_info("%s to hex = %s", dec, result);

  my_bin_to_oct(bin, result);
  pr_info("0b%s to oct = %s", bin, result);

  my_oct_to_hex(oct, result);
  pr_info("0o%s to hex = %s", oct, result);

  my_hex_to_oct(hex, result);
  pr_info("0x%s to oct = %s", hex, result);

  my_bin_to_dec(bin, result);
  pr_info("0b%s to dec = %s", bin, result);
  return 0;
}

void cleanup_module(void) { pr_info("Exit module\n"); }

MODULE_LICENSE("GPL");
