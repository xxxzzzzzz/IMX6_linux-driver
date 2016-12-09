
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include<string.h>
#include <math.h>
void adxl345_read_average(short *x,short *y,short *z,unsigned char times);
float adxl345_Get_angle(short x,short y,short z,unsigned char dir);


static int fd,ret;

int main(void){
short x,y,z;
float angx,angy,angz;
fd=open("/dev/i2c_adxl345", O_RDWR|O_RDONLY,0);
if(fd<0){
	
	printf("i2c_adxl345 open failed\n");
	}

while(1){

adxl345_read_average(&x,&y,&z,10);//get accelerated original value

angx=adxl345_Get_angle(x,y,z,1);  //transform angle
angy=adxl345_Get_angle(x,y,z,2);  //transform angle
angz=adxl345_Get_angle(x,y,z,0);  //transform angle
printf("angx = %f angy = %f angz = %f\n",angx,angy,angz);
usleep(50000);
}

close(fd);
return 0;	
}




void adxl345_read_average(short *x,short *y,short *z,unsigned char times){
  unsigned char i;
  short buf[3];
  short tx,ty,tz;
  *x=0;
  *y=0;
  *z=0;
  if(times){
		

  for(i=0;i<times;i++){
  ret=read(fd, buf, sizeof(buf));
  if(ret<0){
	printf("read adxl345 read_driver erro\n");
  }
  tx=buf[0];
  ty=buf[1];
  tz=buf[2];
   *x+=tx;
   *y+=ty;
   *z+=tz;
  }
   *x/=times;
   *y/=times;
   *z/=times;
  }
}



float adxl345_Get_angle(short x,short y,short z,unsigned char dir){
	
	float tempss=0;
	float res=0;
	switch(dir)
	{
          case 0:tempss=sqrt((x*x+y*y))/z;res=atan(tempss);break;
          case 1:tempss=x/sqrt((y*y+z*z));res=atan(tempss);break;
	  case 2:tempss=y/sqrt((x*x+z*z));res=atan(tempss);break;



	}
	return res*(180/3.14);

}


void read_adxl345(float *angx,float *angy,float *angz){
short x,y,z;	
adxl345_read_average(&x,&y,&z,10);
*angx=adxl345_Get_angle(x,y,z,1);
*angy=adxl345_Get_angle(x,y,z,2);
*angz=adxl345_Get_angle(x,y,z,0);

}
