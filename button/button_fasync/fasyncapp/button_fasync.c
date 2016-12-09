#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include <stdlib.h>



int fd;
void my_signal(int synum){
unsigned char key;

read(fd,&key,sizeof(key));

printf("key = %x\n",key);

}

int main(void){
int oflags;

signal(SIGIO,my_signal);
fd =open("/dev/cdata1_button",O_RDWR);
if(fd<0){

printf("open cdata1_button failed\n");
	 
}


fcntl(fd, F_SETOWN, getpid());
oflags = fcntl(fd, F_GETFL);
fcntl(fd, F_SETFL, oflags | FASYNC);

while(1){
sleep(1);
}
return 0;
}
