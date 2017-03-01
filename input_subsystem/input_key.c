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


/*1 creat input_dev struct*/
static struct input_dev *button_dev;

static struct timer_list button_timer;//定义一个定时器，用于按键消抖功能

static irqreturn_t buttons_irq(int irq,void* dev_id){
    //printk("enter buttons irq process\n");
    //printk("irq=%d\n",irq);
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

        input_report_key(button_dev, KEY_L, 1); // 在input子系统里面处理按键这个上报事件input_report_key的函数要执行两次，input_report_key最后一位上报一次1，再上报一次0,这两个数值并不是代表按键按下和松开，0是为了释放input_report_key事件，所以既然是这样的机制，写代码的人就干脆让他做了按下和松开两个判断，其实在代码里面同时执行两次上报事件1然后0，是没有问题的。
        input_sync(button_dev);
   	input_report_key(button_dev, KEY_L, 0);//如果在代码里面执行了一次input_report_key(button_dev, KEY_L, 1); 然后不将input_report_key最后一位置0在上报一次，那么下次中断来了再执行input_report_key(button_dev, KEY_L, 1);是上报不起的。
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
	/*1.分配一个input_dev 结构体*/
	button_dev=input_allocate_device();
	if(!button_dev)
		printk("input_allocate_device erro\n");

	/*2.设置哪一类事件*/
	set_bit(EV_KEY,button_dev->evbit);
	/*3.能产生这类事件里面的哪些事情，它这个就是事件里面还要分很多事情*/
	set_bit(KEY_L,button_dev->keybit);

	/*4.注册*/
	ret=input_register_device(button_dev);
	if(ret){
		printk("input_register_device failed\n");
	}
	/*硬件操作*/
    	init_timer(&button_timer); //初始化结构体定义的定时器
    	button_timer.function = buttons_timer_function;//定时器中断处理函数是哪一个
    	add_timer(&button_timer);//加载定时器到内核
   	gpio_request(GPIO2_22, "input_button");  
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

