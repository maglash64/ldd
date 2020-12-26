#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>  
#include <linux/usb.h>

#define DEVICE_NAME   "usbd0"
#define DEVICE_CLASS  "usbd"
#define DEVICE_VID    0x0951
#define DEVICE_PID    0x1666

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Ankit Sharma");   
MODULE_DESCRIPTION("A simple linux usb driver");  
MODULE_VERSION("0.1");  


const struct usb_device_id usbd_table[] = {
  {
    USB_DEVICE(DEVICE_VID,DEVICE_PID)
  },{}
};

MODULE_DEVICE_TABLE (usb, usbd_table);

static int usbd_probe(struct usb_interface *interface,const struct usb_device_id *id)
{
  printk(KERN_ALERT "USBD PROBE SUCCESS!");
  return 0;
}

static void usbd_disconnect(struct usb_interface *interface)
{
  printk(KERN_ALERT "USBD DISCONNECT SUCCESS!");
}

static struct usb_driver usbd = {
  .name   = "usbd",
  .probe  = usbd_probe,
  .disconnect = usbd_disconnect,
  .id_table = usbd_table,
};


static int __init usbd_init(void)
{
  int result;
  result = usb_register(&usbd);
  if(result < 0)
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
