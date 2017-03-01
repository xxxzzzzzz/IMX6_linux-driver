
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



void *key_thread1();
void *key_thread2();
void *key_input_system();
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

	int i;
	while(1){
	usleep(100);
	}

}
void *key_thread2(){
	int i;
	while(1){
	usleep(100);
	}

}

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
