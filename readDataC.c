/*author: xiangzizhou ,versions: wbed1.0 */
/*weight sensor 4 individual*/


#include "readData.h"
#include "i2c-dev.h"
#include "adxl345_i2c.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>  

int sensors[SENSOR_NUMBER];
const char *sensorName[] =
{
	"/dev/threesboy_weight1"
	,"/dev/threesboy_weight2"
	,"/dev/threesboy_weight3"
	,"/dev/threesboy_weight4"
};

int InitSensors()
{
	/* weight1 open */
	if ((sensors[sensorGravityFirst] = open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{
		printf("APP open weight_class failed\n");
		return 0;
	}
	else
	{
		printf("APP open weight_class success\n");
	}

	/* weight2 open */
	if ((sensors[sensorGravitySencond] = open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{
		printf("APP open weight_class2 failed\n");
		return 0;
	}
	else
	{
		printf("APP open weight_class2 success\n");
	}

	/* weight3 open */
	if ((sensors[sensorGravityThird] = open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{
		printf("APP open weight_class3 failed\n");
		return 0;
	}
	else
	{
		printf("APP open weight_class3 success\n");
	}

	/* weight4 open */
	if ((sensors[sensorGravityFourth] = open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
	{
		printf("APP open weight_class4 failed\n");
		return 0;
	}

	else
	{
		printf("APP open weight_class4 success\n");
	}

	adxl345_init_open(); //open adxl345
	return 1;
}

void getAng(float *angX, float *angY, float *angZ)
{
	short x, y, z;

	adxl345_read_average(&x, &y, &z, 10);//get accelerated original value
	//printf("x is %d, y is %d, z is %d\n", x, y, z);
	*angX = adxl345_Get_angle(x, y, z, 1);  //transform angle
	*angY = adxl345_Get_angle(x, y, z, 2);  //transform angle
	*angZ = adxl345_Get_angle(x, y, z, 0);  //transform angle
}

void getMaxAng(float angx1, float angy1, float angz1, float angx2, float angy2, float angz2)
{
	float angx, angy, angz, angM;

	angx = angx1 - angx2;
	angx = angx < 0 ? 0 - angx : angx;
	angy = angy1 - angy2;
	angy = angy < 0 ? 0 - angy : angy;
	angz = angz2 - angz2;
	angz = angz < 0 ? 0 - angz : angz;

	angM = angx < angy ? angy : angx;
	angM = angM < angz ? angz : angM;

	accelerationsMax = accelerationsMax >(short)angM ? accelerationsMax : (short)angM;
}

void ReadData()
{
	gravityTotal = 0;
	for (int sc = 0; sc < SENSOR_NUMBER; sc++)
	{
		read(sensors[sc], &data[sc], sizeof(&data[sc]));
		data[sc] = data[sc] < 0 ? 0 : data[sc];
		gravityTotal += data[sc];
	}

	float angx1, angy1, angz1, angx2, angy2, angz2;
	int angCount = 10;
	accelerationsMax = 0;
	getAng(&angx1, &angy1, &angz1);
	while (angCount--)
	{
		//usleep(10000);//time parameter set turn sensitivity
		getAng(&angx2, &angy2, &angz2);
		getMaxAng(angx1, angy1, angz1, angx2, angy2, angz2);

		usleep(1000);//time parameter set turn sensitivity
		getAng(&angx1, &angy1, &angz1);
		getMaxAng(angx1, angy1, angz1, angx2, angy2, angz2);
	}
}


void CloseSensors()
{
	close(sensors[sensorGravityFirst]);	//colse weight1
	close(sensors[sensorGravitySencond]);	//colse weight2
	close(sensors[sensorGravityThird]);	//colse weight3
	close(sensors[sensorGravityFourth]);	//colse weight4
	close(adxl345_fd); //close adxl345
}

char *decimalUnShort2String(unsigned short number)
{
	char *strNumber = (char *)malloc(sizeof(char) * 16);
	sprintf(strNumber, "%d", number);
	return strNumber;
}

char* getTimeZone(short zone)
{
	return "+8:00";
}

char *getTime(unsigned short year, unsigned short mon, unsigned short day, unsigned short hou, unsigned short mini, unsigned short sec, short zone)
{
	char *time = (char *)malloc(sizeof(char) * 64);
	memset(time, 0, 64);
	strcat(time, decimalUnShort2String(year));
	strcat(time, "-");
	strcat(time, decimalUnShort2String(mon));
	strcat(time, "-");
	strcat(time, decimalUnShort2String(day));
	strcat(time, " ");
	strcat(time, decimalUnShort2String(hou));
	strcat(time, ":");
	strcat(time, decimalUnShort2String(mini));
	strcat(time, ":");
	strcat(time, decimalUnShort2String(sec));
	strcat(time, getTimeZone(zone));

	return time;
}

char *GetCurrentTime()
{
	time_t timep = time(NULL);
	//time(&timep);
	struct tm *t = gmtime(&timep);
	year = t->tm_year + 1900;
	month = t->tm_mon + 1;
	day = t->tm_mday;
	hour = t->tm_hour;
	minute = t->tm_min;
	second = t->tm_sec;
	char *time = (char *)malloc(sizeof(char) * 64);
	memset(time, 0, 64);
	strcat(time, getTime(year, month, day, hour, minute, second, 8));
	return time;
}
