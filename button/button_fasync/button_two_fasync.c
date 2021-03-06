
/*author: xiangzizhou ,button*/

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
#include <asm/uaccess.h>
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include <asm/irq.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/wait.h>　
#include <linux/sched.h>
#include <linux/poll.h>

MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))
#define GPIO2_22 IMX_GPIO_NR(2, 22)  /*SW5*/
#define GPIO6_15 IMX_GPIO_NR(6, 15)  /*SW3*/

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
static volatile int ev_prass=0;
static struct fasync_struct *buttomn_fasync;
static struct timer_list button_timer;//定义一个定时器，用于按键消抖功能



static int major;
unsigned char key_value;
static irqreturn_t buttons_irq(int irq,void* dev_id){

//    printk("enter buttons irq process\n");
  //  printk("irq=%d\n",irq);
     /*10ms后启动定时器*/ 
    mod_timer(&button_timer,jiffies+HZ/100);
    return IRQ_HANDLED;
}

static void buttons_timer_function(unsigned long data){
    if((gpio_get_value(GPIO2_22)!=0)&&(gpio_get_value(GPIO6_15)!=0))
	    return 0;

    if(gpio_get_value(GPIO2_22)==0){


	    printk("SW5 loosen\n");
	    key_value=0x05;


    
    }
    else if(gpio_get_value(GPIO6_15)==0){
	    printk("SW3 loosen\n");
            key_value=0x03;
    }
	    
    ev_prass=1;
    wake_up_interruptible(&button_waitq);
    kill_fasync(&buttomn_fasync,SIGIO,POLL_IN);

}

/*button1 open*/
static int button_open(struct inode *inode, struct file *file)
{     
    int irqres;
    irqres = request_irq(gpio_to_irq(GPIO2_22),buttons_irq,IRQF_TRIGGER_FALLING,"SW5",1);
    irqres = request_irq(gpio_to_irq(GPIO6_15),buttons_irq,IRQF_TRIGGER_FALLING,"SW3",2);
    init_timer(&button_timer); 
    button_timer.function = buttons_timer_function;
    add_timer(&button_timer);
    return 0; //this return important if not appear printf luanma	
}

/*button1 read to user*/
static ssize_t button_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
	if(count !=1){
		return -EINVAL;
	}      
        wait_event_interruptible(button_waitq,ev_prass);

	if(copy_to_user(buf,&key_value,1)!=0){

		return -EFAULT;
	}
        ev_prass=0;
        return 0;
}

	

int button1_release(struct inode *inode, struct file *file){
        printk("close button1\n");
	free_irq(gpio_to_irq(GPIO2_22),1);
	free_irq(gpio_to_irq(GPIO6_15),2);
	gpio_free(GPIO2_22);
	gpio_free(GPIO6_15);
        return 0;

}

static unsigned  button_poll(struct file *filp, struct poll_table *wait){
        unsigned int mask=0;
        poll_wait(filp,&button_waitq,wait);
	if (ev_prass){

	mask |= POLLIN | POLLRDNORM;

	}


        return mask;
}


static int button_fasync(int fd, struct file *filp, int on){

printk("button_fasync\n");
return fasync_helper(fd, filp, on, &buttomn_fasync); 

}

static struct file_operations button_fops = {

.owner = THIS_MODULE,
.open = button_open,
.read = button_read,
.release = button1_release,
.poll = button_poll,
.fasync = button_fasync,
};




static struct cdev button_dev;

static struct class *button_class;

static int __init button_init(void){
int result;
dev_t dev;

printk("enter button \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "button");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "button");
     major = MAJOR(dev);
						
     } 



/*button1 add*/
    cdev_init(&button_dev, &button_fops);
    result = cdev_add(&button_dev, dev, 1);
    button_class=class_create(THIS_MODULE, "button");

    device_create(button_class,NULL,MKDEV(major,0),NULL,"cdata1_button");

    return 0;
}





static int __exit button_exit(void){

    printk("exit button \n");

    device_destroy(button_class, MKDEV(major,0));
    
    class_destroy(button_class);
    
    cdev_del(&button_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    


    return 0;
}


module_init(button_init);
module_exit(button_exit);
