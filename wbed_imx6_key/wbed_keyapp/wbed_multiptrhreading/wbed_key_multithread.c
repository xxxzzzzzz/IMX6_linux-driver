/*wbed多线程按键子系统操作 作者：向仔州*/
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <time.h>  
#include <linux/input.h>
#include <pthread.h>



void *key_thread1();//线程key_thread1
void *key_thread2();//线程key_thread2
void *key_input_system();//线程key_input_system

int main(){
	int ret;
	pthread_t id1,id2,id3;
	pthread_create(&id1,NULL,(void *)key_thread1,NULL);
	if(ret){
		printf("key_thread1 create failed\n");
		return 1;
	}

	
	pthread_create(&id2,NULL,(void *)key_thread2,NULL);
	if(ret){
		printf("key_thread2 create failed\n");
		return 1;
	}

		
	pthread_create(&id3,NULL,(void *)key_input_system,NULL);
	if(ret){
		printf("key_thread2 create failed\n");
		return 1;
	}
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	
	return 0;	
}



void *key_thread1(){

	while(1){
	}

}
void *key_thread2(){
	while(1){
	}

}


/*1.在使用该按键设备的时候要定义一个input的结构体,因为我使用的是input子系统框架;
  2.在线程key_input_system里面打开event2，因为按键驱动程序注册的时候生成了event2设备节点，所以操作event2设备节点就是操作wbed IMX6主板上的按键
  3.event2文件打开后，用read函数来读取按键状态，按下按键状态返回1，没按下按键状态返回0; 
  4.在没按下按键的时候read函数将使该线程进入阻塞，此时该线程进入休眠模式;
  5.该代码经过我在多线程和多进程下测试，可以确定按键事件是优先上报的，既不会打断其他程序的正常运行也可以及时响应按键事件。*/
void *key_input_system(){

  int fd;
  int ret;
  struct input_event *keyEvent;
  
  keyEvent = malloc(sizeof(struct input_event));
  fd=open("/dev/input/event2",O_RDWR);
  if(fd<0){
  printf("open input event2 erro\n");	  
  }

  memset(keyEvent, 0, sizeof(struct input_event));
  
  while(1){
  ret = read(fd,keyEvent,sizeof(struct input_event));
  if(ret<0){
	printf("read keyEvent erro\n");
  }

  if((keyEvent->code==KEY_L)&&(keyEvent->value==1)){
     	  
 	printf("enter AP mode\n");	  
    }
  }

		
  free(keyEvent);
  close(fd);

}
