/*author: xiangzizhou ,versions: wbed1.0 */
/*input 1 not demarcate app, 
 *input 2 demarcate app , 
 *input 3 after demarcate app*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 


int demarcate();
void No_demarcate();
void demarcate_weight();
int main(int arc,char *argv[]){


	if(arc!=2){
	printf("input erro\n");
	return -1;
	}
switch(atoi(argv[1]))
{
	case 1:No_demarcate();break;
	case 2:demarcate();break;
	case 3:demarcate_weight();break;

default:
    break;
}	



return 0;
}
	



void No_demarcate(){
	
int weight1,weight2,weight3,weight4,chanshu;
unsigned int data1=0,data2=0,data3=0,data4=0;
unsigned int totalweight;


/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}


/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight2_class failed\n");}
else {printf("APP open weight2_class success\n");}


/*weight3 open sensor*/

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight3_class failed\n");}
else {printf("APP open weight3_class success\n");}


/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
  {printf("APP open weight4_class failed\n");}
else {printf("APP open weight4_class success\n");}


while(1){
	
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value

 totalweight=(data1+data2+data3+data4);

printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %d\n",data1,data2,data3,data4,totalweight);
usleep(10000);
}
close(weight1);
close(weight2);
close(weight3);
close(weight4);
}


/////////////////////////////////////////////////////////get demarcate ////////////////////////


int demarcate(){
 
 int weight1,weight2,weight3,weight4,weight_oefficient_file;
 unsigned int data1=0,data2=0,data3=0,data4=0;
 int totalweight,length;
 unsigned int weight_f=5000;
 float medain,weight_oefficient;
/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}


/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight2_class failed\n");}
else {printf("APP open weight2_class success\n");}


/*weight3 open sensor*/

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight3_class failed\n");}
else {printf("APP open weight3_class success\n");}


/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
  {printf("APP open weight4_class failed\n");}
else {printf("APP open weight4_class success\n");}


	
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value

 totalweight=(data1+data2+data3+data4);

//printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %d\n",data1,data2,data3,data4,totalweight);
printf("wait get weight\n");
do{
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 totalweight=(data1+data2+data3+data4);
 printf("out wait totalweight = %d\n",totalweight);
 sleep(1);
}while(totalweight<2000);
/////////////weight stability data get//////////// 
 sleep(1);
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 totalweight=(data1+data2+data3+data4);

printf("enter calputer weight oefficiebnt\n");
medain=(float)(weight_f-totalweight)/(float)totalweight;  //demarcate calculate
printf("medain out = %f\n",medain);
weight_oefficient=1+medain;//demarcate calculat

printf("write weight_oefficient =  %f\n",weight_oefficient);

if((weight_oefficient_file = creat("/weight_oefficient.txt",0600))<0)
	{printf("weight_oefficient_file creat failed\n");}
	else 
	printf("weight_oefficient_file creat success\n");

if((weight_oefficient_file =open("/weight_oefficient.txt",O_RDWR,0600))<0)
	{printf("weight_oefficient_file open failed\n");}
	else
	 printf("weight_oefficient_file open success\n");

if(length=write(weight_oefficient_file,&weight_oefficient,sizeof(&weight_oefficient))==-1)
	{printf("write weight_oefficient.txt failed\n");}
	else
	printf("write weight_oefficient.txt success\n");




close(weight1);
close(weight2);
close(weight3);
close(weight4);
close(weight_oefficient_file);

return 0;

}
















/////////////////////////////////////////read demarcate after calculat weight data/////////////////////////////////////////


void demarcate_weight(){
int length;	
int weight1,weight2,weight3,weight4,weight_oefficient_file;
float weight_oefficient,totalweight;
unsigned int data1=0,data2=0,data3=0,data4=0;
if((weight_oefficient_file =open("/weight_oefficient.txt",O_RDWR,0600))<0)
	{printf("weight_oefficient_file open failed\n");}
	else
	printf("weight_oefficient_file open success\n");

	if((length = read(weight_oefficient_file,&weight_oefficient,sizeof(&weight_oefficient))))
	{printf("read oefficient success\n");}
	else 
	printf("read oefficient failed\n");
        
close(weight_oefficient_file);



/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}


/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight2_class failed\n");}
else {printf("APP open weight2_class success\n");}


/*weight3 open sensor*/

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight3_class failed\n");}
else {printf("APP open weight3_class success\n");}


/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
  {printf("APP open weight4_class failed\n");}
else {printf("APP open weight4_class success\n");}
printf("read weight_oefficient = %f\n",weight_oefficient);

while(1){
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 totalweight=(data1+data2+data3+data4)*weight_oefficient;
printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %f\n",data1,data2,data3,data4,totalweight);
sleep(1);
} 

close(weight1);
close(weight2);
close(weight3);
close(weight4);
}
