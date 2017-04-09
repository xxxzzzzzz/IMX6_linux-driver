
/*wbed 单线程人体红外传感器输入子系统; 作者：向仔州*/
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <time.h>  
#include <linux/input.h>


int main(){
  int fd;
  int ret;
  struct input_event *keyEvent;
  
  keyEvent = malloc(sizeof(struct input_event));
  fd=open("/dev/input/event3",O_RDWR);
  if(fd<0){
  printf("open input event3 erro\n");	  
  }

  memset(keyEvent, 0, sizeof(struct input_event));
  while(1){
  ret = read(fd,keyEvent,sizeof(struct input_event));
  if(ret<0){
	printf("read heat_relase erro\n");
  }


/*如果有人进入红外区域，就会收到event事件，进入一次就会执行if里面的函数一次，就用下面这个条件判断就可以了，不需要打开上面上报原理的打印段*/  
  if((keyEvent->code==KEY_H)&&(keyEvent->value==1)){
     	  
 	printf("enter heat release \n");	  
    }
  }

  free(keyEvent);
  close(fd);


}
