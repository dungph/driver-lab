#ifndef __VCHAR_IOCTL_CMD_H__
#define __VCHAR_IOCTL_CMD_H__ 1

#include <asm/ioctl.h>

#define BUFFER_SIZE 256

#define IOCTL_ENCRYPT_CAESAR _IOC(_IOC_WRITE, 'k', 1, sizeof(long))
#define IOCTL_DECRYPT_CAESAR _IOC(_IOC_WRITE, 'k', 2, sizeof(long))
#define IOCTL_ENCRYPT_REPLACE _IOC(_IOC_WRITE, 'k', 3, 256)
#define IOCTL_DECRYPT_REPLACE _IOC(_IOC_WRITE, 'k', 4, 256)

#endif
