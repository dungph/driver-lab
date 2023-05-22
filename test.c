#include "vchar_ioctl_cmd.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int8_t buffer[BUFFER_SIZE];
int fd;
void open_dr() { fd = open("/dev/vchar_dev", O_RDWR); }
int main() {
  char option;
  printf("**************************************************\n");
  printf("*******user application to test char driver*******\n");
  while (1) {
    printf("****Please Enter the Option******\n");
    printf("1. (Re)open\n");
    printf("2. Write buffer\n");
    printf("3. Read buffer\n");
    printf("4. Encrypt caesar\n");
    printf("5. Decrypt caesar\n");
    printf("6. Encrypt replace\n");
    printf("7. Decrypt replace\n");
    printf("0. Exit                 \n");
    printf("*********************************\n");
    scanf(" %c", &option);
    printf("Your Option = %c\n", option);

    int key_caesar;
    char key_replace[] = "abcdefghijklmnopqrstuvwxyz";

    switch (option) {
    case '1':
      // Chu y truyen dung ten file thiet bi
      if (fd) {
        close(fd);
      }
      open_dr();
      if (fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
      }
      break;
    case '2':
      printf("Enter the text to write into buffer:");
      scanf("  %[^\t\n]s", buffer);
      printf("Data Writing ...");
      write(fd, buffer, strlen(buffer) + 1);
      printf("Done!\n");
      break;
    case '3':
      printf("Data Reading ...");
      read(fd, buffer, 1024);
      printf("Done!\n\n");
      printf("Data = %s\n\n", buffer);
      break;
    case '4':
      printf("Enter number for encryption key:");
      scanf("  %d", &key_caesar);
      ioctl(fd, IOCTL_ENCRYPT_CAESAR, key_caesar);
      printf("Done!\n\n");
      break;
    case '5':
      printf("Enter number for decryption key:");
      scanf("  %d", &key_caesar);
      ioctl(fd, IOCTL_DECRYPT_CAESAR, key_caesar);
      printf("Done!\n\n");
      break;
    case '6':
      printf("Enter encryption key (%s):", key_replace);
      scanf("  %s", key_replace);
      ioctl(fd, IOCTL_ENCRYPT_REPLACE, key_replace);
      printf("Done!\n\n");
      break;
    case '7':
      printf("Enter decryption key (%s):", key_replace);
      scanf("  %s", key_replace);
      ioctl(fd, IOCTL_DECRYPT_REPLACE, key_replace);
      printf("Done!\n\n");
      break;
    case '0':
      close(fd);
      exit(1);
      break;
    default:
      printf("Enter Valid option = %c\n", option);
      break;
    }
  }
  close(fd);
}
