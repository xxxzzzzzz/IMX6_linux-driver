#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include<string.h>
struct ds1302_write_time{
unsigned int sec;    //write sec
unsigned int min;    //write min
unsigned int hour;   //write hour
unsigned int day;    //write day
unsigned int month;  //write month
unsigned int year;   //write year
};


struct ds1302_readtime{
unsigned int sec;    //read sec
unsigned int min;    //read min
unsigned int hour;   //read hour
unsigned int day;    //read day
unsigned int month;  //read month
unsigned int year;   //read year
};



int main(void){
int fd;
struct ds1302_write_time dstime;//write time struct init
struct ds1302_readtime ds1302_readtm;//read time struct init


/*time value*/
dstime.sec=20;
dstime.min=59;
dstime.hour=23;
dstime.day=31;
dstime.month=12;
dstime.year=10;

fd=open("/dev/threesboy_RTC", O_RDWR); //open ds1302 
if(fd<0){
	
printf("ds1302 open failed!");	
}
if(write(fd,&dstime,sizeof(dstime))<0){
  printf("write file erro!\n");
}

while(1){
read(fd, &ds1302_readtm, sizeof(ds1302_readtm));//read temp  value


printf("sec=%d : min=%d : hours=%d : day=%d : month=%d : year=%d\n ",ds1302_readtm.sec ,ds1302_readtm.min,ds1302_readtm.hour,ds1302_readtm.day,ds1302_readtm.month,ds1302_readtm.year);
sleep(1);
}

colse(fd);
return 0;
}
