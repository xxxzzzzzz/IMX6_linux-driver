/*1hm_sensor crib versions: wbed1.0
* author : 向仔州*/
/*1hm_sensor 4 application*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <math.h>
 
#define weight_oefficient 1.659

void weight11();
void weight22();
void weight33();
void weight44();


int main(int arc,char *argv[]){

int weight1,weight2,weight3,weight4,chanshu;
unsigned int wei[4];
unsigned int data1=0,data2=0,data3=0,data4=0;

unsigned int totalweight,totalweight2;
if(arc!=2){
chanshu=0;
}
else
chanshu=atoi(argv[1]);

switch(chanshu){
	case 1:weight11();break;
	case 2:weight22();break;
	case 3:weight33();break;
	case 4:weight44();break; 
	default: break;
}
/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0){

       printf("APP open weight1_class failed\n"); 

     }
else {

	printf("APP open weight1_class success\n");
     }

/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0){

       printf("APP open weight2_class failed\n"); 

     }
else {

	printf("APP open weight2_class success\n");
     }

/*weight3 open sensor*/

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0){

       printf("APP open weight3_class failed\n"); 

     }
else {

	printf("APP open weight3_class success\n");
     }
/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0){

       printf("APP open weight4_class failed\n"); 

     }
else {

	printf("APP open weight4_class success\n");
     }
while(1){
	
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value

 totalweight=(data1+data2+data3+data4) * weight_oefficient;

printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %d\n",data1,data2,data3,data4,totalweight);
usleep(10000);
}
close(weight1);
close(weight2);
close(weight3);
close(weight4);
return 0;
}






void weight11(){

int weight1,data1;
float data1f;
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0){
     printf("APP open weight1_class failed\n"); 
     }
else {
     printf("APP open weight1_class success\n");
     }
while(1){
     read(weight1, &data1, sizeof(data1));//read weight1 value
     data1f=data1* weight_oefficient ;
     printf("weight1 : %d\n",(int)data1f);
        }
}



void weight22(){

int weight2,data2;

float data2f;
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0){
     printf("APP open weight2_class failed\n"); 
     }
else {
     printf("APP open weight2_class success\n");
     }
while(1){
     read(weight2, &data2, sizeof(data2));//read weight2 value
     data2f=data2*weight_oefficient;
     printf("weight2 : %d\n",(int)data2f);
        }
}

void weight33(){

int weight3,data3;

float data3f;
if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0){
     printf("APP open weight3_class failed\n"); 
     }
else {
     printf("APP open weight3_class success\n");
     }
while(1){
     read(weight3, &data3, sizeof(data3));//read weight3 value
     data3f=data3*weight_oefficient;
     printf("weight3 : %d\n",(int)data3f);
        }
}

void weight44(){

int weight4,data4;

float data4f;
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0){
     printf("APP open weight4_class failed\n"); 
     }
else {
     printf("APP open weight4_class success\n");
     }
while(1){
     read(weight4, &data4, sizeof(data4));//read weight4 value
     data4f=data4*weight_oefficient;
     printf("weight4 : %d\n",(int)data4f);
        }
}

