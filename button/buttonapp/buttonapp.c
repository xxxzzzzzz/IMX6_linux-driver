#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>


int main(){
int fd,res;
unsigned char key;

fd = open("/dev/cdata1_button",O_RDWR);

if(fd<0){

	printf("open cdata1_button failed\n");
}

while(1){

	res = read(fd,&key,sizeof(key));
	printf("key = %d",key);

}
close(fd);
return 0;

}
