#ifndef __ADXL345_I2C_H
#define __ADXL345_I2C_H

#define DEVICE_ID   0X00
#define THRESH_TAP  0X1D
#define OFSX        0X1E 
#define OFSY        0X1F
#define OFSZ        0X20
#define DUR         0X21
#define Latent      0X22
#define Window      0X23
#define THRESH_ACK  0X24
#define THRESH_INACT 0X25
#define TIME_INACT  0X26
#define ACT_INACT_CTL 0X27
#define THRESH_FF   0X28
#define TIME_FF     0X29
#define TAP_AXES    0X2A
#define ACT_TAP_STATUS 0X2B
#define BW_RATE     0X2C
#define POWER_CTL   0X2D
#define INT_ENABLE  0X2E
#define INT_MAP     0X2F
#define INT_SOURCE  0X30
#define DATA_FORMAT 0X31
#define DATA_X0     0X32
#define DATA_X1     0X33
#define DATA_Y0     0X34
#define DATA_Y1     0X35
#define DATA_Z0     0X36
#define DATA_Z1     0X37
#define FIFO_CTL    0X38
#define FIFO_STATUS 0X39
#define ADXL_READ   0XA7
#define ADXL_WRITE  0xA6
int adxl345_fd;
int adxl345_fd2;
unsigned char adxl345_init(void);
void adxl345_read_xyz(short *x,short *y, short *z);
void adxl345_read_average(short *x,short *y,short *z,unsigned char times);
float adxl345_Get_angle(short x,short y,short z,unsigned char dir);
void read_adxl345(float *angx,float *angy,float *angz);
void adxl345_init_open();


void adxl345_init_open_2();
unsigned char adxl345_init_2(void);
void adxl345_read_xyz_2(short *x,short *y, short *z);
void adxl345_read_average_2(short *x,short *y,short *z,unsigned char times);

#endif
