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

fd =creat("/home/xiang/IMX6/IMX-driver/1HM_weightwbed/weightapp/1hm_weight_biaoding_app/biaoding.txt",0600);
if(fd = -1){
printf("txt creat failed\n");
				
}

else{
printf("create txt succeed\n");
				
}

if((fd=open("/home/xiang/IMX6/IMX-driver/1HM_weightwbed/weightapp/1hm_weight_biaoding_app/biaoding.txt",O_RDWR,0600))<0){
 printf("open txt failed\n");	

  }
else
 printf("opne txt success\n");



	close(fd);

}
