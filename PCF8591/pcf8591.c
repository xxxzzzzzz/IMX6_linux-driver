#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "i2c-dev.h"
#include <math.h>
#include <unistd.h>



int main(){
int fd;
char data=0;

fd=open("/dev/i2c-2",O_RDWR |  O_NDELAY);
if(fd<0){
   printf("i2c-2 open erro\n");
}

if(ioctl(fd,I2C_SLAVE_FORCE,0x90)<0){
       printf("ioctl PCF8591 addr failed!\n");
       exit(1);	
}

while(1){

i2c_smbus_write_byte(fd,0x41);
data=i2c_smbus_read_byte(fd);
printf("adc = %d\n",data);
}

close(fd);
}
