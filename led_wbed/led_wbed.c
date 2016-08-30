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
MODULE_LICENSE("Dual BSD/GPL");

#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))

#define LED_internet_gpio IMX_GPIO_NR(4, 5)
#define LED_system_start IMX_GPIO_NR(7, 12)
#define LED_app_internet IMX_GPIO_NR(7, 11)

static unsigned long led_array[3]={
	LED_internet_gpio,
	LED_system_start,
	LED_app_internet,
        };


static int major=0;

static int led_wbed_open (struct inode *inode, struct file *file){
int ret;

printk("enter led_wbed_open\n");
//................led_internet.........init
ret = gpio_request_one(LED_internet_gpio,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
if(ret){printk("gpio LED_internet_gpio request failed\n");}
else {printk("gpio LED_internet_gpio  request success\n");}
//................LED_system_start.........init
ret = gpio_request_one(LED_system_start,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
if(ret){printk("gpio LED_system_start request failed\n");}
else {printk("gpio LED_system_start  request success\n");}
//................LED_app_internet.........init
ret = gpio_request_one(LED_app_internet,GPIOF_OUT_INIT_LOW, "fec-phy-reset");

if(ret){printk("gpio LED_app_internet request failed\n");}
else {printk("gpio LED_app_internet  request success\n");}
return 0;
}


static long led_wbed_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
printk("enter led_wbed_ioctl\n");

switch(cmd){
        case 0:gpio_set_value(led_array[arg],0);break;//led_internet_on

	case 1:gpio_set_value(led_array[arg],1);break;//led_internet_off
	default:
	
	return -EINVAL;

}

return 0;
}


int led_wbed_release(struct inode *inode, struct file *file){
gpio_free(LED_internet_gpio);

printk("enter led_wbed_close\n");
return 0;
}


static struct file_operations led_wbed_fops = {

   .owner = THIS_MODULE,
   .open = led_wbed_open,
   .unlocked_ioctl = led_wbed_ioctl,
   .release = led_wbed_release,


};


static struct cdev  led_wbed_dev;
static struct class *led_wbed_class;

static int ledwbed_init(void){
  int result;
  dev_t dev;

  printk("enter ledwbed_init\n");

  if(major){
   dev=MKDEV(major,0);
   result = register_chrdev_region(dev, 1, "leds");
		       
           }

   else {
   result = alloc_chrdev_region(&dev, 0, 1, "leds");
   major = MAJOR(dev);

        }
             
  cdev_init(&led_wbed_dev,&led_wbed_fops);
  result = cdev_add(&led_wbed_dev,dev,1);
  led_wbed_class=class_create(THIS_MODULE, "ledwbed");
  device_create(led_wbed_class,NULL,MKDEV(major,0),NULL,"threesboy_led");

   return 0;
}

static int ledwbed_exit(void){

   printk("exit ledwbed_init\n");
   device_destroy(led_wbed_class, MKDEV(major,0));
   class_destroy(led_wbed_class);
   cdev_del(&led_wbed_dev);
   unregister_chrdev_region(MKDEV(major,0), 1);

   return 0;

}



	



module_init(ledwbed_init);
module_exit(ledwbed_exit);


