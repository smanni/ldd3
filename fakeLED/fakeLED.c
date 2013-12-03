/* MISC module that drives a fake LED */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <asm/uaccess.h>

#define MAX_STRING_LENGTH 30

static uint8_t LED_status = 0;			/* start with LED off */

char LED_strings[2][MAX_STRING_LENGTH] = {"The LED is OFF\r\n", "The LED is ON\r\n"};

static ssize_t fakeLED_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	if(count != 1)
	{
		pr_err("fakeLED: invalid LED value");
		return -EINVAL;	
	}
	
	switch(buf[0])
	{
		case '0':
			LED_status = 0;
			pr_info("fakeLED: LED OFF");
			break;
			
		case '1':
			LED_status = 1;
			pr_info("fakeLED: LED ON");
			break;

		case 't':
			LED_status = !LED_status;
			pr_info("fakeLED: LED TOGGLED (%d)", LED_status);
			break;
						
		default:
			pr_err("fakeLED: invalid LED value");
			return -EINVAL;			
	}
	
	return count;
}

static ssize_t fakeLED_read(struct file *f, char __user *buf, size_t count, loff_t *ppos)
{
	loff_t off = *ppos;
	       
	if (off >= MAX_STRING_LENGTH) 
	{
		return 0; /* EOF */
	}
	
	if (copy_to_user(buf, LED_strings[LED_status], MAX_STRING_LENGTH))
	{
		return -EFAULT;
	}
	
	/* move the offset forth */
	*ppos += MAX_STRING_LENGTH;
	
	return MAX_STRING_LENGTH;	
}
                       
static struct file_operations fakeLED_fops = {
	.owner = THIS_MODULE,
	.write = fakeLED_write,
	.read = fakeLED_read,
};

static struct miscdevice fakeLED_misc = {
	.name = "LED",
	.fops = &fakeLED_fops,
};

static int __init fakeLED_init(void)
{
	pr_info("fakeLED: module registered\n");
	return misc_register(&fakeLED_misc);
}

static void __exit fakeLED_exit(void)
{
	pr_info("fakeLED: module unregistered\n");
	misc_deregister(&fakeLED_misc);
}

module_init(fakeLED_init);
module_exit(fakeLED_exit);
MODULE_AUTHOR("Stefano Manni");
MODULE_LICENSE("GPL and additional rights");
