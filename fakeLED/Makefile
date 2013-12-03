# My first char device driver

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= fakeLED.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


depend .depend dep:
	$(CC) $(EXTRA_CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
