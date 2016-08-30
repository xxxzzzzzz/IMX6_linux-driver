#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>

int main(){
int fd;
fd = open("/dev/gpio_test", 0);
if(fd<0){
printf("gpio_test open failed!");	

  }
while(1){
ioctl(fd,1,0);
sleep(1);
ioctl(fd,0,0);
sleep(1);
}

return 0;
}
