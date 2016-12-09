obj-m +=adxl345_dt_driver.o

KDIR:=/home/xiang/IMX6/fsl-linux
PWD=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o
