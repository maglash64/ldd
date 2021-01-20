#include <linux/init.h>   // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/fs.h>     // Header for the Linux file system support
#include <linux/usb.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "usbd"
#define DEVICE_CLASS "usbd_class"
#define DEVICE_VID 0x0483
#define DEVICE_PID 0xff11

struct usbd_device
{
  struct usb_device *dev;
  struct usb_class_driver class;
  unsigned char buffer[2];
};

static struct usbd_device usbd_dev;

static int usbd_open(struct inode *i, struct file *filp)
{
  return 0;
}

static int usbd_close(struct inode *i, struct file *filp)
{
  return 0;
}

static ssize_t usbd_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
  int ret,cnt;
  ret = usb_bulk_msg(usbd_dev.dev,usb_rcvbulkpipe(usbd_dev.dev,0x81),usbd_dev.buffer,2,&cnt,1000);
  if(ret)
  {
    printk(KERN_ALERT "USBD BULK READ FAILED!, ERR: %d\n",ret);
    return ret;
  }
  if(copy_to_user(buff,usbd_dev.buffer,2))
  {
    return -EFAULT;
  }
  printk(KERN_ALERT "USBD READ %s\n",usbd_dev.buffer);
  return cnt;
}

static ssize_t usbd_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
  int ret,cnt;
  if(copy_from_user(usbd_dev.buffer,buff,2))
  {
    return -EFAULT;
  }
  ret = usb_bulk_msg(usbd_dev.dev,usb_sndbulkpipe(usbd_dev.dev,0x01),usbd_dev.buffer,2,&cnt,1000);
  if(ret)
  {
    printk(KERN_ALERT "USBD BULK WIRTE FAILED!, ERR: %d\n",ret);
    return ret;
  }
  printk(KERN_ALERT "USBD WROTE %s\n",usbd_dev.buffer);
  return cnt;
}

static struct file_operations usbd_fops =
    {
        .owner = THIS_MODULE,
        .open = usbd_open,
        .release = usbd_close,
        .read = usbd_read,
        .write = usbd_write,
};

const struct usb_device_id usbd_table[] = {
    {USB_DEVICE(DEVICE_VID, DEVICE_PID)}, {}};

MODULE_DEVICE_TABLE(usb, usbd_table);

static int usbd_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
  int ret;

  usbd_dev.dev = interface_to_usbdev(interface);
  usbd_dev.class.name = "usbd";
  usbd_dev.class.fops = &usbd_fops;

  if ((ret = usb_register_dev(interface, &usbd_dev.class)) < 0)
  {
    printk(KERN_ALERT "USBD PROBE FAILED!, CANNOT REGISTER DEVICE\n");
  }else
  {
    printk(KERN_ALERT "USBD PROBE SUCCESS! minor: %d\n",interface->minor);
  }
  
  return ret;
}

static void usbd_disconnect(struct usb_interface *interface)
{
  usb_deregister_dev(interface,&usbd_dev.class);
  printk(KERN_ALERT "USBD DISCONNECT SUCCESS!\n");
}

static struct usb_driver usbd = {
    .name = "usbd",
    .probe = usbd_probe,
    .disconnect = usbd_disconnect,
    .id_table = usbd_table,
};

static int __init usbd_init(void)
{
  int result;
  result = usb_register(&usbd);
  if (result < 0)
  {
    printk(KERN_ALERT "error registering : usbd");
    return -1;
  }
  return 0;
}

static void __exit usbd_exit(void)
{
  usb_deregister(&usbd);
}

module_init(usbd_init);
module_exit(usbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ankit Sharma");
MODULE_DESCRIPTION("A simple linux usb driver");
MODULE_VERSION("0.1");
