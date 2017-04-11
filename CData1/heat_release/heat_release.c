#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/of_gpio.h>

#include <asm/irq.h> 
#include <asm/io.h>
MODULE_LICENSE("Dual BSD/GPL");

#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))
#define GPIO1_05 IMX_GPIO_NR(1, 5)  /*heat release*/
/*#define GPIO1_05 IMX_GPIO_NR(4, 13)*/  /*i2c2_sda  heat release*/


/* creat input_dev struct*/
static struct input_dev *heat_release_dev;


static irqreturn_t heat_release_irq(int irq,void* dev_id){

//	    printk("heat release\n");
    /*if(gpio_get_value(GPIO1_05)==1){*/

	 //   printk("heat release\n");

        input_report_key(heat_release_dev, KEY_H, 1); 
        input_sync(heat_release_dev);
   	input_report_key(heat_release_dev, KEY_H, 0);
   	input_sync(heat_release_dev);
   /* }*/
  /* else{
	 printk("heat release else\n"); 
   	 input_report_key(heat_release_dev, KEY_H, 0);
   	 input_sync(heat_release_dev);

    }*/
    return IRQ_RETVAL(IRQ_HANDLED);
}



static int __init input_heat_release_init(void){
	int irqres;
	int ret;
	heat_release_dev=input_allocate_device();
	if(!heat_release_dev)
		printk("input_allocate_device erro\n");

	set_bit(EV_KEY,heat_release_dev->evbit);
	set_bit(KEY_H,heat_release_dev->keybit);

	ret=input_register_device(heat_release_dev);
	if(ret){
		printk("input_register_device failed\n");
		}
    
   	
	gpio_request(GPIO1_05, "heat_release");  
	gpio_direction_input(GPIO1_05);
	irqres = request_irq(gpio_to_irq(GPIO1_05),heat_release_irq,IRQF_TRIGGER_RISING,"heat_release",NULL);
	printk("input_heat_release_init\n");
	return 0;
}



static void __exit input_heat_release_exit(void){
		
	free_irq(gpio_to_irq(GPIO1_05),NULL);
	gpio_free(GPIO1_05);
	input_unregister_device(heat_release_dev);
	input_free_device(heat_release_dev);
	printk("input_heat_release_exit\n");
	

}



module_init(input_heat_release_init);
module_exit(input_heat_release_exit);

