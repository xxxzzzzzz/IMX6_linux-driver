ADC:pcf8591.o
	arm-fsl-linux-gnueabi-gcc -o $@ pcf8591.o -static
pcf8591.o:pcf8591.c i2c-dev.h 
	arm-fsl-linux-gnueabi-gcc -c  $^ -static


clean:
	rm -rf *.o
