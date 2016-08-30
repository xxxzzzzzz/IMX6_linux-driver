#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/*data[0] is temperature ,data[1] is humi*/


int main(void){
int fd;
unsigned char data801s[2];
if((fd=open("/dev/threesboy_pcf8591", O_RDWR | O_NDELAY))<0){ //open temphumi

printf("open pcf8591 failed!");	
}

while(1){ 
 read(fd,data801s,sizeof(data801s));
 printf("vibration = %d  vibration22 = %d \n",data801s[0],data801s[1]);
usleep(10000);
}
close(fd);
return 0;
}
