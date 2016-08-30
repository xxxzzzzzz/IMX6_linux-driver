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
int main(void){
short x,y,z,x2,y2,z2;
float angx,angy,angz,angx2,angy2,angz2,angx_2A,angy_2A,angz_2A,angx_2B,angy_2B,angz_2B,angx_2_1,angy_2_1,angz_2_1,angx_A,angy_A,angz_A,angx_B,angy_B,angz_B;

adxl345_init_open(); //open adxl345
adxl345_init_open_2();

/*senser 1 A*/
adxl345_read_average(&x,&y,&z,10);//get accelerated original value
angx_A=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_A=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_A=adxl345_Get_angle(x,y,z,0);  //transform angle

/*senser 2 A*/
adxl345_read_average_2(&x2,&y2,&z2,10);
angx_2A=adxl345_Get_angle(x2,y2,z2,1);  //transform angle
angy_2A=adxl345_Get_angle(x2,y2,z2,2);  //transform angle
angz_2A=adxl345_Get_angle(x2,y2,z2,0);  //transform angle

while(1){
 
   /*accelerated senser 1*/
adxl345_read_average(&x,&y,&z,10);//get accelerated original value
angx_B=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_B=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_B=adxl345_Get_angle(x,y,z,0);  //transform angle
angx=angx_A-angx_B;
angy=angy_A-angy_B;
angz=angz_A-angz_B;


  /*accelerated senser 2*/
adxl345_read_average_2(&x2,&y2,&z2,10);
angx_2B=adxl345_Get_angle(x2,y2,z2,1);  //transform angle
angy_2B=adxl345_Get_angle(x2,y2,z2,2);  //transform angle
angz_2B=adxl345_Get_angle(x2,y2,z2,0);  //transform angle
angx2=angx_2A-angx_2B;
angy2=angy_2A-angy_2B;
angz2=angz_2A-angz_2B;
usleep(50000);//time parameter set turn sensitivity
printf("x = %f : y = %f : z = %f | x2 = %f : y2 = %f : z2 = %f\n ",angx,angy,angz,angx2,angy2,angz2); //get difference



    /*senser 1 A*/
adxl345_read_average(&x,&y,&z,10);//get accelerated original value
angx_A=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_A=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_A=adxl345_Get_angle(x,y,z,0);  //transform angle

   /*senser 2 A*/
adxl345_read_average_2(&x2,&y2,&z2,10);
angx_2A=adxl345_Get_angle(x2,y2,z2,1);  //transform angle
angy_2A=adxl345_Get_angle(x2,y2,z2,2);  //transform angle
angz_2A=adxl345_Get_angle(x2,y2,z2,0);  //transform angle


angx=angx_A-angx_B;
angy=angy_A-angy_B;
angz=angz_A-angz_B;


angx2=angx_2A-angx_2B;
angy2=angy_2A-angy_2B;
angz2=angz_2A-angz_2B;

usleep(50000);//time parameter set turn sensitivity
printf("x = %f : y = %f : z = %f | x2 = %f : y2 = %f : z2 = %f\n ",angx,angy,angz,angx2,angy2,angz2); //get difference
}
close(adxl345_fd); //close adxl345
close(adxl345_fd2); //close adxl345_2

return 1;
}


