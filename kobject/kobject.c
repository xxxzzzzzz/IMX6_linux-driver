#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/platform_device.h> 
#include <linux/kobject.h>
MODULE_AUTHOR("David Xie");
MODULE_LICENSE("Dual BSD/GPL");
 
static ssize_t kobj_test_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
	char str[20]="hello world";
	printk("enter kobj_test_show.\n");
 	return sprintf(buf, "%s\n", str);
}
 
static ssize_t kobj_test_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk("kobj_test_store\n");
	printk("write: %s\n",buf);
	
	return count;
}

static struct kobj_attribute bL_switcher_active_attr = __ATTR(RGB_led, 0644,kobj_test_show, kobj_test_store);//在目录里面创建的文件名RGB_led就是一个文件名
 
static struct attribute *bL_switcher_attrs[] = {
	&bL_switcher_active_attr.attr,
	NULL,

};

static struct attribute_group bL_switcher_attr_group = {
	.attrs = bL_switcher_attrs,

};

static struct kobject *bL_switcher_kobj;
static int kobj_test_init(void)
{
	int ret;
	printk("kboject test init.\n");
	bL_switcher_kobj = kobject_create_and_add("RGB_LED_switch", bL_switcher_kobj);//在/sys/目录下创建一个目录 RGB_LED_switch就是目录名称
	if (!bL_switcher_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(bL_switcher_kobj, &bL_switcher_attr_group);
	if (ret)
		kobject_put(bL_switcher_kobj);
	return 0;

}
 
static int  kobj_test_exit(void)
{
	printk("kobject test exit.\n");
	return 0;

}
 
module_init(kobj_test_init);
module_exit(kobj_test_exit);
