#include <linux/device.h>

#include "at24c02_rw.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_4 IMX_GPIO_NR(1, 4) 
#define GPIO1_5 IMX_GPIO_NR(1, 5) 

#define clk GPIO1_4
#define sda GPIO1_5


void i2c_start(void){
gpio_direction_output(sda,1);
gpio_direction_output(clk,1);
gpio_set_value(sda,0);
udelay(5);
gpio_set_value(clk,0);
udelay(5);
}


void i2c_stop(void){
gpio_direction_output(sda,0);
gpio_direction_output(clk,1); 
udelay(5); 
gpio_set_value(sda,1);
udelay(5);
gpio_set_value(clk,0);
udelay(5);
}


 void ACK1(void){
	
gpio_set_value(sda,0);
udelay(5);
gpio_set_value(clk,1);
udelay(5);
gpio_set_value(clk,0);

}



static void Nack(void){
	
gpio_set_value(sda,1);
udelay(5);
gpio_set_value(clk,1);
udelay(5);
gpio_set_value(clk,0); 
}


void send(unsigned char Data){
	unsigned char BitCounter=8;       /*位数控制*/ 
	unsigned char temp;     /*中间变量控制*/ 
	do
	{
		
		temp=Data;
		gpio_set_value(clk,0);
		udelay(5);
		if((temp&0x80)==0x80)/* 如果最高位是1*/ 
                gpio_set_value(sda,1);
		else
		gpio_set_value(sda,0);
		 
		gpio_set_value(clk,1);
		temp=Data<<1;
		Data=temp;
                BitCounter--; 



	}while(BitCounter);


       gpio_set_value(clk,0);
}


unsigned char Read(void){
	

	unsigned char i,bytedata=0;
	
	
        gpio_direction_input(sda);
	for(i=0;i<8;i++){
        gpio_set_value(clk,1);
	udelay(5);
	bytedata<<=1;

	   if(gpio_get_value(sda)) bytedata|=0x01;  /*temp的最低位置1*/ 

        gpio_set_value(clk,0);
	udelay(5);
	}
	gpio_direction_output(sda,1); 

	return(bytedata); 
}


void write_rom(unsigned char addr,unsigned char data){
	gpio_direction_output(sda,1);
	i2c_start();
        send(0xa0);
	ACK1();
	send(addr);
	ACK1();
        send(data);
	ACK1();
	i2c_stop();

}


unsigned char read_rom(unsigned char addr){
unsigned char temp;
gpio_direction_output(sda,1);
	i2c_start();
	send(0xa0);
	ACK1();
	send(addr);
	ACK1();
	i2c_start();
	send(0xa1);
        ACK1();
	temp=Read();
   gpio_set_value(clk,0);
        Nack();
	i2c_stop();
}
