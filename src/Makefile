KDIR := /home/source/kernel  # Your kernel path
PWD := $(shell pwd)

# For cross compile
COMPILER = arm-linux-gnueabihf-
ARCH = arm

obj-m = my_device.o

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) CROSS_COMPILE=$(COMPILER) ARCH=$(ARCH) modules

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
