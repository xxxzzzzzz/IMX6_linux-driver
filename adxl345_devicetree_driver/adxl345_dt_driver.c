#include<linux/init.h> 
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/
#include <linux/module.h>
/*定义module_param module_param_array的头文件*/
//#include <linux/moduleparam.h>
///*定义module_param module_param_array中perm的头文件*/
////#include <linux/stat.h>
///*三个字符设备函数*/
#include <linux/fs.h>
///*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
///*定义字符设备的结构体*/
#include <linux/cdev.h>
///*分配内存空间函数头文件*/
////#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
//
///*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/i2c.h>
MODULE_LICENSE("Dual BSD/GPL");


/////////////////////////////////////////////register///////////////////
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
/////////////////////////////////////////////register///////////////////



static int major=0;
struct i2c_client *adxl345_client;

static int adxl345_open(struct inode *inode, struct file *file){
static unsigned char ID;
static unsigned char reg_addr[][2]={
	{DEVICE_ID,0},
	{DATA_FORMAT,0x2b},
	{BW_RATE,0x0e},
	{POWER_CTL,0x28},
	{INT_ENABLE,0x00},
	{OFSX,0x00},
	{OFSY,0x00},
	{OFSZ,0x00}
	
};
printk("enter adxl345_open\n");
i2c_master_send(adxl345_client, *reg_addr, 1);
i2c_master_recv(adxl345_client, &ID, 1);  // 接收寄存器的值
if(ID==0xE5){
printk("access adxl345 success\n");	
}
else
printk("access adxl345 failed\n");

i2c_master_send(adxl345_client, *(reg_addr+1),2);
i2c_master_send(adxl345_client, *(reg_addr+2),2);
i2c_master_send(adxl345_client, *(reg_addr+3),2);
i2c_master_send(adxl345_client, *(reg_addr+4),2);
i2c_master_send(adxl345_client, *(reg_addr+5),2);
i2c_master_send(adxl345_client, *(reg_addr+6),2);
i2c_master_send(adxl345_client, *(reg_addr+7),2);

return 0;
}

static ssize_t adxl345_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
	
static unsigned char get_vaule_addr;
static short x,y,z,app_buf[3];
static unsigned char original_buf[6];
get_vaule_addr = 0x32;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf, 1);  // 接收寄存器的值


get_vaule_addr = 0x33;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf+1, 1);  // 接收寄存器的值

get_vaule_addr = 0x34;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf+2, 1);  // 接收寄存器的值

get_vaule_addr = 0x35;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf+3, 1);  // 接收寄存器的值

get_vaule_addr = 0x36;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf+4, 1);  // 接收寄存器的值

get_vaule_addr = 0x37;
i2c_master_send(adxl345_client, &get_vaule_addr,1);
i2c_master_recv(adxl345_client, original_buf+5, 1);  // 接收寄存器的值


x=(short)(((unsigned int)original_buf[1]<<8)+original_buf[0]);
y=(short)(((unsigned int)original_buf[3]<<8)+original_buf[2]);
z=(short)(((unsigned int)original_buf[5]<<8)+original_buf[4]);
//printk("kernel x = %d y = %d z = %d\n",x,y,z);
app_buf[0]=x;
app_buf[1]=y;
app_buf[2]=z;
if(copy_to_user(buf, app_buf, sizeof(app_buf))!=0){
	
   return -EFAULT;
   }
return 0;
}

int adxl345_release(struct inode *inode, struct file *file){

printk("enter adxl345_close\n");
return 0;
}


static struct file_operations adxl345_fops = {

   .owner = THIS_MODULE,
   .open = adxl345_open,
   .read = adxl345_read,
   .release = adxl345_release,


};


static struct cdev  adxl345_dev;
static struct class *adxl345_class;
static int adxl345_probe(struct i2c_client *client,const struct i2c_device_id *id){

  int result;
  dev_t dev;
  printk("adxl345dt probe\n");

  adxl345_client = client;
	  


  if(major){
   dev=MKDEV(major,0);
   result = register_chrdev_region(dev, 1, "adxl345");
		       
           }

   else {
   result = alloc_chrdev_region(&dev, 0, 1, "adxl345");
   major = MAJOR(dev);

        }
             
  cdev_init(&adxl345_dev,&adxl345_fops);
  result = cdev_add(&adxl345_dev,dev,1);
  adxl345_class=class_create(THIS_MODULE, "adxl345");
  device_create(adxl345_class,NULL,MKDEV(major,0),NULL,"i2c_adxl345");
	return 0;
}

static int adxl345_remove(struct platform_device *pdev){
	
   printk("adxl345dt remove\n");
   device_destroy(adxl345_class, MKDEV(major,0));
   class_destroy(adxl345_class);
   cdev_del(&adxl345_dev);
   unregister_chrdev_region(MKDEV(major,0), 1);
	return 0;

}


static const struct i2c_device_id adxl345_id[] = {
	{ "adxl345", 0  },
	{  }

};

static struct of_device_id adxl345dt[]={
	{ .compatible = "adxl345",},
	{},

	};

MODULE_DEVICE_TABLE(of, adxl345dt);


static struct i2c_driver adxl345_driver = {
	.driver = {
	.name= "adxl345",
	.owner= THIS_MODULE,
	.of_match_table= adxl345dt,
											
	},
	.id_table= adxl345_id,
	.probe= adxl345_probe,
	.remove = adxl345_remove,
};



static int adxl345_init(void){
  printk("enter adxl345_init\n");
  i2c_add_driver(&adxl345_driver);
  return 0;
}

static int adxl345_exit(void){

   printk("enter adxl345_exit\n");
   i2c_del_driver(&adxl345_driver);
   return 0;

}



	



module_init(adxl345_init);
module_exit(adxl345_exit);


