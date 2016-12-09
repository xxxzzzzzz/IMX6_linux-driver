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

#define gpio_test IMX_GPIO_NR(3, 26)



static int major=0;

static int led_wbed_open (struct inode *inode, struct file *file){
int ret;

printk("enter gpio_test_open\n");
ret = gpio_request_one(gpio_test,GPIOF_OUT_INIT_LOW, "fec-phy-reset");

//................led_internet.........init
if(ret){printk("gpio LED_internet_gpio request failed\n");}
else {printk("gpio LED_internet_gpio  request success\n");}
return 0;
}


static long led_wbed_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
printk("enter gpio_test_ioctl\n");

switch(cmd){
        case 0:gpio_set_value(gpio_test,0);break;//gpio_test_on

	case 1:gpio_set_value(gpio_test,1);break;//gpio_test_off
	default:
	
	return -EINVAL;

}

return 0;
}


int led_wbed_release(struct inode *inode, struct file *file){
gpio_free(gpio_test);

printk("enter gpio_test_close\n");
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

static int gpiowbed_init(void){
  int result;
  dev_t dev;

  printk("enter gpiowbed_init\n");

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
  led_wbed_class=class_create(THIS_MODULE, "gpiotest");
  device_create(led_wbed_class,NULL,MKDEV(major,0),NULL,"gpio_test");

   return 0;
}

static int gpiowbed_exit(void){

   printk("exit gpiowbed_init\n");
   device_destroy(led_wbed_class, MKDEV(major,0));
   class_destroy(led_wbed_class);
   cdev_del(&led_wbed_dev);
   unregister_chrdev_region(MKDEV(major,0), 1);

   return 0;

}



	



module_init(gpiowbed_init);
module_exit(gpiowbed_exit);


