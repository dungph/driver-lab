PWD := $(shell pwd)

obj-m += pcf_lcd.o

default:
	make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf- -C $(PWD)/linux M=$(PWD) modules
	dtc -@ -I dts -O dtb -o overlay.dtbo overlay.dts
