#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>
#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-device.h>

static int success = 0;
struct video_device  vdev;
struct v4l2_device    v4l2_dev;


static int v4l2_open(struct file *filp)
{
  printk(KERN_ALERT "v4l2test opened.\n");
  return 0;
}

static int v4l2_release(struct file *filp)
{
  printk(KERN_ALERT "v4l2test closed.\n");
  return 0;
}

static const struct v4l2_file_operations v4l2_fops = {
  .owner = THIS_MODULE,
  .open = v4l2_open,
  .release = v4l2_release,
};

static int __init v4l2test_init(void)
{
  int err = -ENOMEM;
 
  sprintf(v4l2_dev.name,"v4l2test");

  err = v4l2_device_register(NULL,&v4l2_dev);

  if(err)
  {
    printk(KERN_ALERT "Cannot register v4l2_device");
    return err;
  }

  sprintf(vdev.name,"v4l2test");
  vdev.release = video_device_release;
  vdev.v4l2_dev = &v4l2_dev;
  vdev.fops = &v4l2_fops;
  vdev.device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_READWRITE;

  err = video_register_device(&vdev,VFL_TYPE_GRABBER,-1);
  if(err)
  {
    v4l2_device_unregister(&v4l2_dev);
    printk(KERN_ALERT "error, cannot register vdev");
    return err;
  }

  success = 1;
  return 0;
}

static void __exit v4l2test_exit(void)
{
  if(success)
  {
    v4l2_device_unregister(&v4l2_dev);
  }
}

module_init(v4l2test_init);
module_exit(v4l2test_exit);


MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Ankit Sharma");   
MODULE_DESCRIPTION("A simple linux v4l2 driver");  
MODULE_VERSION("0.1");  