ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello3.o
ccflags-y += -g
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello3.ko hello3.ko.unstripped
	$(CROSS_COMPILE)strip -g hello3.ko

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif
