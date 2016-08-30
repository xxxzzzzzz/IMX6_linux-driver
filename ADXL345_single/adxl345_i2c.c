#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "i2c-dev.h"
#include "adxl345_i2c.h"
#include <math.h>
#include <unistd.h>


//int adxl345_fd;


void adxl345_init_open(){

adxl345_fd=open("/dev/i2c-1",O_RDWR);
if(adxl345_fd<0){
	printf("i2c-1 open erro\n");


}
if(ioctl(adxl345_fd,I2C_SLAVE,0x53)<0){
	
	printf("ioctl erro\n");
	exit(1);


}

while(adxl345_init()){
	printf("adxl345 init erro\n");


}

	
}




unsigned char adxl345_init(void){
	

	if(i2c_smbus_read_word_data(adxl345_fd,DEVICE_ID)==0XE5){
	i2c_smbus_write_word_data(adxl345_fd,DATA_FORMAT,0X2B);
        i2c_smbus_write_word_data(adxl345_fd,BW_RATE,0X0E);
        i2c_smbus_write_word_data(adxl345_fd,POWER_CTL,0X28);
	i2c_smbus_write_word_data(adxl345_fd,INT_ENABLE,0X00);
	i2c_smbus_write_word_data(adxl345_fd,OFSX,0X00);
	i2c_smbus_write_word_data(adxl345_fd,OFSY,0X00);
        i2c_smbus_write_word_data(adxl345_fd,OFSZ,0X00);
	return 0;
	}
	return 1;

}



void adxl345_read_xyz(short *x,short *y, short *z){
	unsigned char i;
	 unsigned char buf[6];
	// for(i=0;i<6;i++){

	buf[0]=i2c_smbus_read_word_data(adxl345_fd,0x32);
        buf[1]=i2c_smbus_read_word_data(adxl345_fd,0x33);
	buf[2]=i2c_smbus_read_word_data(adxl345_fd,0x34);
	buf[3]=i2c_smbus_read_word_data(adxl345_fd,0x35);
	buf[4]=i2c_smbus_read_word_data(adxl345_fd,0x36);
	buf[5]=i2c_smbus_read_word_data(adxl345_fd,0x37);
	*x=(short)(((unsigned int)buf[1]<<8)+buf[0]);
	*y=(short)(((unsigned int)buf[3]<<8)+buf[2]);
	*z=(short)(((unsigned int)buf[5]<<8)+buf[4]);


	// }

}


void adxl345_read_average(short *x,short *y,short *z,unsigned char times){
  unsigned char i;
  short tx,ty,tz;
  *x=0;
  *y=0;
  *z=0;
  if(times){
		

  for(i=0;i<times;i++){
   adxl345_read_xyz(&tx,&ty,&tz);
   *x+=tx;
   *y+=ty;
   *z+=tz;
   //usleep(5000);
  }
   *x/=times;
   *y/=times;
   *z/=times;
  }
}

float mySqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86- (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy

	return 1/x;
}

float adxl345_Get_angle(short x,short y,short z,unsigned char dir){
	
	float tempss=0;
	float res=0;
	//printf("before: x is %d, y is %d,z is %d, res is %f, tempss is %f\n", x, y, z, res, tempss);
	switch(dir)
	{
          case 0:tempss=sqrt((x*x+y*y))/z;res=atan(tempss);break;
          case 1:tempss=x/sqrt((y*y+z*z));res=atan(tempss);break;
	  case 2:tempss=y/sqrt((x*x+z*z));res=atan(tempss);break;



	}
	//printf("$$$$$$$$$$$$$$after: x is %d, y is %d,z is %d, res is %f, tempss is %f\n",x, y, z, res, tempss);
	return res*(180/3.14);

}


void read_adxl345(float *angx,float *angy,float *angz){
short x,y,z;	
adxl345_read_average(&x,&y,&z,10);
*angx=adxl345_Get_angle(x,y,z,1);
*angy=adxl345_Get_angle(x,y,z,2);
*angz=adxl345_Get_angle(x,y,z,0);

}
