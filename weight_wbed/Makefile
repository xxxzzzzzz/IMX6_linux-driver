obj-m +=hx711x4_wbed.o
hx711x4_wbed-objs=HX711x4.o IIC_analog/iic_analog.o IIC_analog/iic_analog2.o IIC_analog/iic_analog3.o  IIC_analog/iic_analog4.o
KDIR:=/home/xiang/IMX6/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/IMX-driver/weight_wbed/IIC_analog


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o
