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
short x,y,z;
float angx_A,angy_A,angz_A,angx_B,angy_B,angz_B,angx,angy,angz;

adxl345_init_open(); //open adxl345
adxl345_read_average(&x,&y,&z,10);//get accelerated original value
angx_A=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_A=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_A=adxl345_Get_angle(x,y,z,0);  //transform angle

while(1){


adxl345_read_average(&x,&y,&z,10); 
angx_B=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_B=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_B=adxl345_Get_angle(x,y,z,0);  //transform angle
angx=angx_A-angx_B;
angy=angy_A-angy_B;
angz=angz_A-angz_B;

printf("x = %f : y = %f : z = %f\n ",angx,angy,angz); //get difference 
usleep(50000);

adxl345_read_average(&x,&y,&z,10); 
angx_A=adxl345_Get_angle(x,y,z,1);  //transform angle
angy_A=adxl345_Get_angle(x,y,z,2);  //transform angle
angz_A=adxl345_Get_angle(x,y,z,0);  //transform angle
angx=angx_A-angx_B;
angy=angy_A-angy_B;
angz=angz_A-angz_B;
printf("x = %f : y = %f : z = %f\n ",angx,angy,angz); //get difference 
usleep(50000); 

}
close(adxl345_fd); //close adxl345

return 1;
}


