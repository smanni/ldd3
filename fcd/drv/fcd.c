/* My first char device driver*/ 

#include "fcd.h"

/* Global variables */

static char device[SIZE]="hi from kernel"; 		// the device
static dev_t first; 							// the first device number 
static struct cdev c_dev; 						// the character device structure
static struct class *cl; 						// the device class

struct file_operations fcd_fops = 
{
	open: fcd_open,
	read: fcd_read,
	write: fcd_write,
	release:fcd_release,
};

/* File operation methods */

int fcd_open(struct inode *inode, struct file *filep)
{
	printk(KERN_NOTICE "%s: open\n", FCD_NAME);
	return 0;
}

int fcd_release(struct inode *inode, struct file *filep)
{
	printk(KERN_NOTICE "%s: release\n", FCD_NAME);
	return 0;
}

ssize_t fcd_read(struct file *filep, char *buff, size_t count, loff_t *offp )
{
	unsigned long not_copied;
	
	printk(KERN_NOTICE "%s: read count = %d", FCD_NAME, count);

	if(count > SIZE)
		count = SIZE;	
	
	not_copied = copy_to_user(buff, device, count);
    if (not_copied != 0)
        return -EFAULT;

	return count;
}

ssize_t fcd_write(struct file *filep, const char *buff, size_t count, loff_t *offp )
{
	unsigned long not_copied;
	
	printk(KERN_NOTICE "%s: write\n", FCD_NAME);
	
	if(count > SIZE)
		count = SIZE;
			
	not_copied = copy_from_user(device, buff, count);
    if (not_copied != 0)
        return -EFAULT;			

	return count;
}

static int fcd_init(void)
{	
	if (alloc_chrdev_region(&first, 0, 1, FCD_NAME) < 0)
	{
		printk(KERN_WARNING "%s: alloc_chrdev_region() failed\n", FCD_NAME);
		return -1;
	}
	
	cl = class_create(THIS_MODULE, FCD_NAME);
	if (cl == NULL)
	{
		printk(KERN_WARNING "%s: class_create() failed\n", FCD_NAME);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	if (device_create(cl, NULL, first, NULL, FCD_NAME) == NULL)
	{
		printk(KERN_WARNING "%s: device_create() failed\n", FCD_NAME);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	cdev_init(&c_dev, &fcd_fops);
	if (cdev_add(&c_dev, first, 1) == -1)
	{
		printk(KERN_WARNING "%s: cdev_add() failed\n", FCD_NAME);
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	printk(KERN_NOTICE "%s: successfully registered\n", FCD_NAME);
	return 0;	
}

static void fcd_cleanup(void)
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_NOTICE "%s: successfully unregistered\n", FCD_NAME);
}
 

module_init(fcd_init);
module_exit(fcd_cleanup);

MODULE_AUTHOR("mannis");
MODULE_DESCRIPTION("My first char device driver");
MODULE_LICENSE("Dual BSD/GPL");
