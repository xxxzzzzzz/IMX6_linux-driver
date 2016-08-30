
/*author: xiangzizhou ,versions: wbed1.0 */
/*pcf8591 sensor 4 individual*/

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
#include "pcf8591_rw.h"
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_3 IMX_GPIO_NR(1, 3) 
#define GPIO1_6 IMX_GPIO_NR(1, 6) 

#define clk GPIO1_3
#define sda GPIO1_6

static int major;
static int pcf8591_open (struct inode *inode, struct file *file){
 static int ret;
        ret = gpio_request_one(sda,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("sda request failed!\n");}
	ret = gpio_request_one(clk,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("clk request failed!\n");}
        printk("pcf8591 open\n");

        return 0; //this return important if not appear printf luanma	
}

/*pcf8591 read to user*/
static ssize_t pcf8591_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static  unsigned char data[2];

        
          data[0] = PCF8591_WR_Reg();	
	  data[1] = PCF8591_WR_Reg2();
   if(copy_to_user(buf, data, count)!=0){
      return -EFAULT;
         } 
      return 1;
}




int pcf8591_release(struct inode *inode, struct file *file){
      gpio_free(GPIO1_3);
      gpio_free(GPIO1_6);
      printk("close pcf85911\n");
      return 0;

}









static struct file_operations pcf8591_fops = {

.owner = THIS_MODULE,
.open = pcf8591_open,
.read = pcf8591_read,
.release = pcf8591_release,

};


static struct cdev pcf8591_dev;
static struct class *pcf8591_class;

static int pcf8591_init(void){
int result;
dev_t dev;

printk("enter pcf8591 \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "pcf8591");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "pcf8591");
     major = MAJOR(dev);
						
     } 



/*pcf8591 add*/
    cdev_init(&pcf8591_dev, &pcf8591_fops);
    result = cdev_add(&pcf8591_dev, dev, 1);



    pcf8591_class=class_create(THIS_MODULE, "pcf8591");

    device_create(pcf8591_class,NULL,MKDEV(major,0),NULL,"threesboy_pcf8591");

    return 0;
}





static int pcf8591_exit(void){

    printk("exit pcf8591 \n");

    device_destroy(pcf8591_class, MKDEV(major,0));
    
    class_destroy(pcf8591_class);
    
    cdev_del(&pcf8591_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(pcf8591_init);
module_exit(pcf8591_exit);
