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
#define GPIO2_22 IMX_GPIO_NR(2, 22)  /*SW5*/
#define GPIO6_15 IMX_GPIO_NR(6, 15)  /*SW3*/


/* creat input_dev struct*/
static struct input_dev *button_dev;

static struct timer_list button_timer;//定义一个定时器，用于按键消抖功能

static irqreturn_t buttons_irq(int irq,void* dev_id){
     /*10ms后启动定时器*/ 
    mod_timer(&button_timer,jiffies+HZ/100);
    return IRQ_RETVAL(IRQ_HANDLED);
}

/*定时器中断处理函数*/
static void buttons_timer_function(unsigned long data){
	
    if((gpio_get_value(GPIO2_22)!=0)&&(gpio_get_value(GPIO6_15)!=0))
	    return;

    if(gpio_get_value(GPIO2_22)==0){

	    printk("SW5 loosen\n");

        input_report_key(button_dev, KEY_L, 1); 
        input_sync(button_dev);
   	input_report_key(button_dev, KEY_L, 0);
   	input_sync(button_dev);
    }
    else if(gpio_get_value(GPIO6_15)==0){
	  
	printk("SW3 loosen\n");
   	input_report_key(button_dev, KEY_L, 1);
   	input_sync(button_dev);
   	input_report_key(button_dev, KEY_L, 0);
   	input_sync(button_dev);
    }
    else{
	    
   	 input_report_key(button_dev, KEY_L, 0);
   	 input_sync(button_dev);

    }
	   


}


static int __init input_keys_init(void){
	int irqres;
	int ret;
	button_dev=input_allocate_device();
	if(!button_dev)
		printk("input_allocate_device erro\n");

	set_bit(EV_KEY,button_dev->evbit);
	set_bit(KEY_L,button_dev->keybit);

	ret=input_register_device(button_dev);
	if(ret){
		printk("input_register_device failed\n");
		}
    
	init_timer(&button_timer); 
    	button_timer.function = buttons_timer_function;
    	add_timer(&button_timer);
   	
	gpio_request(GPIO2_22, "wbed_imx6_button");  
	gpio_direction_input(GPIO2_22);
	irqres = request_irq(gpio_to_irq(GPIO2_22),buttons_irq,IRQF_TRIGGER_FALLING,"SW5",NULL);
	irqres = request_irq(gpio_to_irq(GPIO6_15),buttons_irq,IRQF_TRIGGER_FALLING,"SW3",NULL);
	printk("input_keys_init\n");
	return 0;
}



static void __exit input_keys_exit(void){
		
	free_irq(gpio_to_irq(GPIO2_22),NULL);
	free_irq(gpio_to_irq(GPIO6_15),NULL);
	gpio_free(GPIO2_22);
	gpio_free(GPIO6_15);
	del_timer(&button_timer);
	input_unregister_device(button_dev);
	input_free_device(button_dev);
	printk("input_keys_exit\n");
	

}



module_init(input_keys_init);
module_exit(input_keys_exit);

