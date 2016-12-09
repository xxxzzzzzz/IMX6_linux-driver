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
int shuzi=555444;
char buf[] ="\n";
char buf2[]="write data";
ssize_t lenght;

if((fd=open("/home/xiang/IMX6/IMX-driver/1HM_weightwbed/weightapp/1hm_weight_biaoding_app/biaoding.txt",O_RDWR,0600))<0){
 printf("open txt failed\n");	

  }
else
 printf("opne txt success\n");

lenght=write(fd,&shuzi,sizeof(&shuzi));
if(lenght == -1)
	{printf("write txt erro\n");}
else
	{printf("Write Function OK!\n");}


lenght=write(fd,buf,strlen(buf));
if(lenght == -1)
	{printf("write txt erro\n");}
else
	{printf("Write Function OK!\n");}


lenght=write(fd,buf2,strlen(buf2));
if(lenght == -1)
	{printf("write txt erro\n");}
else
	{printf("Write Function OK!\n");}

close(fd);


}
