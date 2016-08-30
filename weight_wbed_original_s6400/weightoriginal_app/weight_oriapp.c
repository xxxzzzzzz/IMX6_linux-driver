#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 


void weight11();
void weight22();
void weight33();
void weight44();


int main(int arc,char *argv[]){

int weight1,weight2,weight3,weight4,chanshu;
unsigned int wei[4];
unsigned int data1=0,data2=0,data3=0,data4=0;
unsigned int org,org2,org3,org4;//original value init
float coefficient=1.09;  //coefficient setting value
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
    read(weight1, &org, sizeof(org));//read weight1 original value
    read(weight2, &org2, sizeof(org2));//read weight2 original value
    read(weight3, &org3, sizeof(org3));//read weight3 original value
    read(weight4, &org4, sizeof(org4));//read weight4 original value

while(1){
	
    read(weight1, &data1, sizeof(&data1));//read weight1 value
    read(weight2, &data2, sizeof(&data2));//read weight2 value
    read(weight3, &data3, sizeof(&data3));//read weight3 value
    read(weight4, &data4, sizeof(&data4));//read weight4 value
    data1=(data1-org)*10; //weight1 actual value and original value difference value
    data2=(data2-org2)*10;//weight2 actual value and original value difference value
    data3=(data3-org3)*10;//weight3 actual value and original value difference value
    data4=(data4-org4)*10;//weight4 actual value and original value difference value
    totalweight=(data1+data2+data3+data4)*coefficient;//total weight

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

int weight1,original,data;

if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0){
     printf("APP open weight1_class failed\n"); 
     }
else {
     printf("APP open weight1_class success\n");
     }
     
     read(weight1, &original, sizeof(original));//read weight1 value

while(1){
     read(weight1, &data, sizeof(data));//read weight1 value
     printf("original weight1 %d",data);
     data=data-original;
     printf("weight1 : %d\n",data);
        }
}



void weight22(){

int weight2,data2,original2;

if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0){
     printf("APP open weight2_class failed\n"); 
     }
else {
     printf("APP open weight2_class success\n");
     }
     read(weight2, &original2, sizeof(original2));//read weight1 value
while(1){
     read(weight2, &data2, sizeof(data2));//read weight2 value
     printf("original weight2 %d",data2);
     data2=data2-original2;
     printf("weight2 : %d\n",data2);
        }
}

void weight33(){

int weight3,data3,original3;

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0){
     printf("APP open weight3_class failed\n"); 
     }
else {
     printf("APP open weight3_class success\n");
     }
     read(weight3, &original3, sizeof(original3));//read weight1 value
while(1){
     read(weight3, &data3, sizeof(data3));//read weight3 value
     printf("original weight3 %d",data3);
     data3=data3-original3;
     printf("weight3 : %d\n",data3);
        }
}

void weight44(){

int weight4,data4,original4;

if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0){
     printf("APP open weight4_class failed\n"); 
     }
else {
     printf("APP open weight4_class success\n");
     }
     read(weight4, &original4, sizeof(original4));//read weight1 value
while(1){
     read(weight4, &data4, sizeof(data4));//read weight4 value
     printf("original weight4 %d",data4);
     data4=data4-original4;
     printf("weight4 : %d\n",data4);
        }
}

