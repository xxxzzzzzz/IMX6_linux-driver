obj-m +=i2c_dt_test.o

KDIR:=/home/xiang/IMX6/fsl-linux
PWD=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o
