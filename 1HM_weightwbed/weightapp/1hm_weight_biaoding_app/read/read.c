
//标准输入输出头文件
#include <stdio.h>
//文件操作函数头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


void main(){
	
int fd;
ssize_t length;
char buf_read[1000];
int buf_s;


if((fd=open("/home/xiang/IMX6/IMX-driver/1HM_weightwbed/weightapp/1hm_weight_biaoding_app/biaoding.txt",O_RDWR,0600))<0)
{printf("open txt failed\n");}
else
 printf("opne txt success\n");

if(length = read(fd,&buf_s,1000))
{printf("read txt success\n");}
else
{printf("read txt failed\n");}
printf("%d\n",buf_s);

/*if(length = read(fd,&buf_s,1000))
{printf("read txt success\n");}
else
{printf("read txt failed\n");}
printf("%d\n",buf_s);*/

close(fd);

}
