/* My first char device driver*/ 

#ifndef _FCDRIVER_H
#define _FCDRIVER_H

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>


#define FCD_NAME			"fcd"
#define FCD_MAJOR 			115
#define FCD_MINOR 			0
#define FCD_NR_DEVS 		4    /* fcd0 through fcd3 */

#define SIZE				20

/* Function prototipes */
static int fcd_init(void);
static void fcd_cleanup(void);
int fcd_open(struct inode *inode, struct file *filep);
int fcd_release(struct inode *inode, struct file *filep);
ssize_t fcd_read(struct file *filep, char *buff, size_t count, loff_t *offp );
ssize_t fcd_write(struct file *filep, const char *buff, size_t count, loff_t *offp );

#endif   // _FCWRITER_H
