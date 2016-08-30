#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>

#define  internet 0
#define  system_start 1
#define  app_internet 2
#define  led_on       1
#define  led_off      0

int main(){
int fd;

fd = open("/dev/threesboy_led", 0); //open 3 led
if(fd<0){
printf("led open failed!");	
        }

while(1){
ioctl(fd,led_on,internet);      //borad to internet connect led off/on
ioctl(fd,led_on,system_start);  //borad start over control led off/on
ioctl(fd,led_on,app_internet);  //app to internet connect led off/on
sleep(1);
ioctl(fd,led_off,internet);     //borad to internet connect led off/on
ioctl(fd,led_off,system_start); //borad start over control led off/on
ioctl(fd,led_off,app_internet); //app to internet connect led off/on 
sleep(1);
}

return 0;
}
