PWD := $(shell pwd)

obj-m += i2c1602.o

default:
	make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf- -C $(PWD)/linux M=$(PWD) modules
