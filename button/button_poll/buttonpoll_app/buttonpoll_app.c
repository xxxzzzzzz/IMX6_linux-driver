#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <poll.h>



int main(){
int fd,res,ret;
unsigned char key;
struct pollfd fds[1];


fd = open("/dev/cdata1_button",O_RDWR);

if(fd<0){

	printf("open cdata1_button failed\n");
}

fds[0].fd = fd;
fds[0].events = POLLIN;

while(1){
        ret  = poll(fds,1,5000);
	if(ret == 0){
		
		printf("time out 5\n");
	}
	else{
	res = read(fd,&key,1);
	printf("key = %d\n",key);
	//printf("jincheng huanq\n");

	}

}
close(fd);
return 0;

}
