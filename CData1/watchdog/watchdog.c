#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <unistd.h>




void main(){

int fd,timeout,timeremaining;
fd = open("/dev/watchdog", O_WRONLY);

if (fd == -1){
	
printf("watchdog open failed\n");	
exit(1);
			    
}

timeout=20;

ioctl(fd, WDIOC_SETTIMEOUT, &timeout);//看门狗是按1秒钟的时间单位递减，我设置的20秒之后重启

while(1){
	

ioctl(fd, WDIOC_GETTIMEOUT, &timeout);//读取看门狗初始时间是否和自己设置的一样
ioctl(fd, WDIOC_GETTIMELEFT, &timeremaining);//驱动没有获取剩余时间的功能
printf("timeout = %d timeremaining = %d\n",timeout,timeremaining);
sleep(1);
ioctl(fd, WDIOC_KEEPALIVE, 0); //使用WDIOC_KEEPALIVE方法喂狗
}

//close(fd);//一般是不需要关闭看门狗的


}
