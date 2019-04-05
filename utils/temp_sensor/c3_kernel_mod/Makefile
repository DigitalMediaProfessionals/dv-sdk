ARCH=$(shell gcc -print-multiarch)

ifeq ($(ARCH), aarch64-linux-gnu)

# For on-board compiling (64-bit ARM)
MAKEARCH = $(MAKE)

BUILD_DIR := $(shell pwd)
VERBOSE = 0
PWD = $(shell pwd)
MOD = zia_c3_temp_mon
KERNELDIR ?= /lib/modules/`uname -r`/build

obj-m := $(MOD).o

all:
	$(MAKEARCH) -C $(KERNELDIR) M=$(PWD) modules

.SILENT:	install

install:
	echo Copying $(MOD).ko to /lib/modules/`uname -r`/
	cp $(MOD).ko /lib/modules/`uname -r`/
	echo depmod -a
	depmod -a
	echo To reload the module, execute: sudo rmmod $(MOD) \&\& sudo modprobe $(MOD)

clean:
	rm -rf *.ko *.o *.order .tmp_vers* *.symvers *.mod.c .dmp*

endif
