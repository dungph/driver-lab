#ifndef __VCHAR_IOCTL_CMD_H__
#define __VCHAR_IOCTL_CMD_H__ 1

#include <asm/ioctl.h>

#define BUFFER_SIZE 256


#define IOCTL_INPUT _IOC(_IOC_WRITE, 'k', 1, BUFFER_SIZE)
#define IOCTL_READ_BIN _IOC(_IOC_READ, 'k', 2, BUFFER_SIZE)
#define IOCTL_READ_OCT _IOC(_IOC_READ, 'k', 3, BUFFER_SIZE)
#define IOCTL_READ_HEX _IOC(_IOC_READ, 'k', 4, BUFFER_SIZE)

#endif
