PWD := $(shell pwd)
obj-m += main.o

default:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
cross:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) SUBDIRS=$(PWD) modules
insmod:
	sudo insmod main.ko
rmmod:
	sudo rmmod main
