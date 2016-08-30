adxl345test:adxl345_i2c.o adxl345app.o 
	arm-fsl-linux-gnueabi-gcc -o $@ adxl345_i2c.o adxl345app.o -lm -static

adxl345_i2c.o:adxl345_i2c.c adxl345_i2c.h i2c-dev.h
	arm-fsl-linux-gnueabi-gcc -c $^ -lm -static

adxl345app.o:adxl345app.c adxl345_i2c.h i2c-dev.h
	arm-fsl-linux-gnueabi-gcc -c $^ -lm -static

clean:
	rm -rf *.o
