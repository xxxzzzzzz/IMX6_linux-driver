obj-m +=button_two_poll.o
KDIR:=/home/xiang/IMX6/fsl-linux


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o
