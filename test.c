#include "vchar_ioctl_cmd.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int8_t buffer[BUFFER_SIZE];
int main() {
  int fd;
  char option;
  printf("**************************************************\n");
  printf("*******user application to test char driver*******\n");
  while (1) {
    printf("****Please Enter the Option******\n");
    printf("        1. Open\n");
    printf("        2. Get exact nanosecond\n");
    printf("        3. Get exact microsecond\n");
    printf("        4. Get relative second\n");
    printf("        6. Exit                 \n");
    printf("*********************************\n");
    scanf(" %c", &option);
    printf("Your Option = %c\n", option);

    switch (option) {
    case '1':
      // Chu y truyen dung ten file thiet bi
      fd = open("/dev/vchar_dev", O_RDWR);
      if (fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
      }
      break;
    case '2':
      printf("Data Reading ...");
      ioctl(fd, IOCTL_GET_NANO_S, buffer);
      printf("Done!\n\n");
      printf("Data = %s\n\n", buffer);
      break;
    case '3':
      printf("Data Reading ...");
      ioctl(fd, IOCTL_GET_MICRO_S, buffer);
      printf("Done!\n\n");
      printf("Data = %s\n\n", buffer);
      break;
    case '4':
      printf("Data Reading ...");
      ioctl(fd, IOCTL_GET_TIME, buffer);
      printf("Done!\n\n");
      printf("Data = %s\n\n", buffer);
      break;
    case '6':
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
