#ifndef __AT24C02_RW_H
#define __AT24C02_RW_H
void i2c_start(void);
void i2c_stop(void);
 void ACK1(void);
static void Nack(void);
void send(unsigned char Data);
unsigned char Read(void);
void write_rom(unsigned char addr,unsigned char data);
unsigned char read_rom(unsigned char addr);


#endif
