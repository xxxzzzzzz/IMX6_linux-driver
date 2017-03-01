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

/*  printf("---------------------------------\n"); 
  printf("type = %d\n",keyEvent->type);
  printf("code = %d\n",keyEvent->code);
  printf("value = %d\n",keyEvent->value);
  printf("\n");*/
  if((keyEvent->code==KEY_L)&&(keyEvent->value==1)){
     	  
 	printf("enter AP mode\n");	  
    }
  }

  free(keyEvent);
  close(fd);


}
