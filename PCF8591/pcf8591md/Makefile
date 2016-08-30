obj-m +=PCF8591MD.o
PCF8591MD-objs=pcf8591.o pcf8591_rw.o 
KDIR:=/home/xiang/IMX6/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/IMX-driver/PCF8591/pcf8591md


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o
