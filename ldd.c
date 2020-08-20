#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>  

#define DEVICE_NAME "ldd"
#define DEVICE_CLASS "ldd_class"

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Ankit Sharma");   
MODULE_DESCRIPTION("A simple Linux char driver");  
MODULE_VERSION("0.1");  


static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static int major;
static int is_dev_open = 0;
static struct class* lddclass = NULL;
static struct device* ldddev = NULL;

static char ldd_msg[256] = "well, hello there..";
static const uint32_t msg_len = 256;

struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_release,
};

static int dev_open(struct inode *inode, struct file *file)
{
  is_dev_open++;
  printk(KERN_ALERT "ldd device opened %d times\n", is_dev_open);
  return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
  printk(KERN_ALERT "ldd device closed\n");
  return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t length, loff_t *offset)
{
  if(!copy_to_user(buffer,ldd_msg,length))
  {
    return 0;
  }
  else
  {
    return -EFAULT;
  }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t length, loff_t *offset)
{
  copy_from_user(ldd_msg,buffer,length);
  return 0;
}

static int __init ldd_init(void)
{
  major = register_chrdev(0, DEVICE_NAME, &fops);

  if (major < 0)
  {
    printk(KERN_ALERT "ldd module loading failed!\n");
    return major;
  }

  lddclass = class_create(THIS_MODULE,DEVICE_CLASS);
  
  if(IS_ERR(lddclass))
  {
    unregister_chrdev(major,DEVICE_NAME);
    printk(KERN_ALERT "Dailed to register ldd driver.\n");
    return PTR_ERR(lddclass);
  }

  ldddev = device_create(lddclass,NULL,MKDEV(major,0),NULL,DEVICE_NAME);
  
  if(IS_ERR(ldddev))
  {
    class_destroy(lddclass);
    unregister_chrdev(major,DEVICE_NAME);
    printk(KERN_ALERT "Dailed to register ldd driver.\n");
    return PTR_ERR(lddclass);
  }

  printk(KERN_ALERT "ldd module loaded into kernel with major no: %d\n", major);

  return 0;
}

static void __exit ldd_exit(void)
{
  device_destroy(lddclass,MKDEV(major,0));
  class_unregister(lddclass);
  class_destroy(lddclass);
  unregister_chrdev(major, DEVICE_NAME);
  printk(KERN_ALERT "ldd module exited from kernel\n");
}

module_init(ldd_init);
module_exit(ldd_exit);
