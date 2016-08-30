#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <math.h>


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

 totalweight=data1+data2+data3+data4;
/* totalweight= abs(totalweight);
 totalweight2=totalweight;
 
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 totalweight=data1+data2+data3+data4;
 totalweight= abs(totalweight);*/
/*weight turn over*/
/*if((totalweight>(totalweight2+300))||(totalweight2>(totalweight+300))){
	printf("fanshen\n");
}*/
/*if((totalweight>(totalweight2+300))){
	
	printf("fanshen\n");

}*/
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

if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0){
     printf("APP open weight1_class failed\n"); 
     }
else {
     printf("APP open weight1_class success\n");
     }
while(1){
     read(weight1, &data1, sizeof(data1));//read weight1 value
     printf("weight1 : %d\n",data1);
        }
}



void weight22(){

int weight2,data2;

if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0){
     printf("APP open weight2_class failed\n"); 
     }
else {
     printf("APP open weight2_class success\n");
     }
while(1){
     read(weight2, &data2, sizeof(data2));//read weight2 value
     printf("weight2 : %d\n",data2);
        }
}

void weight33(){

int weight3,data3;

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0){
     printf("APP open weight3_class failed\n"); 
     }
else {
     printf("APP open weight3_class success\n");
     }
while(1){
     read(weight3, &data3, sizeof(data3));//read weight3 value
     printf("weight3 : %d\n",data3);
        }
}

void weight44(){

int weight4,data4;

if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0){
     printf("APP open weight4_class failed\n"); 
     }
else {
     printf("APP open weight4_class success\n");
     }
while(1){
     read(weight4, &data4, sizeof(data4));//read weight4 value
     printf("weight4 : %d\n",data4);
        }
}

