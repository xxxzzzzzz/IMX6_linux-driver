
/*author: xiangzizhou ,versions: wbed1.0 */
/*at24c02 sensor 4 individual*/

/*包含初始化宏定义的头文件,代码中的module_init和module_exit在此文件中*/
#include <linux/init.h>
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/
#include <linux/module.h>
/*定义module_param module_param_array的头文件*/
//#include <linux/moduleparam.h>
/*定义module_param module_param_array中perm的头文件*/
//#include <linux/stat.h>
/*三个字符设备函数*/
#include <linux/fs.h>
/*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
/*定义字符设备的结构体*/
#include <linux/cdev.h>
/*分配内存空间函数头文件*/
//#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include "at24c02_rw.h"
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_4 IMX_GPIO_NR(1, 4) 
#define GPIO1_5 IMX_GPIO_NR(1, 5) 

#define clk GPIO1_4
#define sda GPIO1_5

static int major;
static int at24c02_open (struct inode *inode, struct file *file){
  int ret;
  unsigned char adc,res=1;
        ret = gpio_request_one(sda,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("sda request failed!\n");}
	ret = gpio_request_one(clk,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("clk request failed!\n");}
        printk("at24c02 open\n");

	    write_rom(0x01,0x50);
        while(1){
            adc = read_rom(0x01);	    
	      printk("adc = %d\n",adc);
	      mdelay(500);
	}
        return 0; //this return important if not appear printf luanma	
}

/*at24c02 read to user*/
static ssize_t at24c02_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static  unsigned char data[2];
static  unsigned char temperature,humidity;

   data[0]=temperature;
   data[1]=humidity;
   if(copy_to_user(buf, data, count)!=0){
      return -EFAULT;
         } 
      return 1;
}




int at24c021_release(struct inode *inode, struct file *file){
      gpio_free(GPIO1_4);
      gpio_free(GPIO1_5);
      printk("close at24c021\n");
      return 0;

}









static struct file_operations at24c02_fops = {

.owner = THIS_MODULE,
.open = at24c02_open,
.read = at24c02_read,
.release = at24c021_release,

};


static struct cdev at24c02_dev;
static struct class *at24c02_class;

static int at24c02_init(void){
int result;
dev_t dev;

printk("enter at24c02 \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "at24c02");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "at24c02");
     major = MAJOR(dev);
						
     } 



/*at24c02 add*/
    cdev_init(&at24c02_dev, &at24c02_fops);
    result = cdev_add(&at24c02_dev, dev, 1);



    at24c02_class=class_create(THIS_MODULE, "at24c02");

    device_create(at24c02_class,NULL,MKDEV(major,0),NULL,"threesboy_pcf8591");

    return 0;
}





static int at24c02_exit(void){

    printk("exit at24c02 \n");

    device_destroy(at24c02_class, MKDEV(major,0));
    
    class_destroy(at24c02_class);
    
    cdev_del(&at24c02_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(at24c02_init);
module_exit(at24c02_exit);
