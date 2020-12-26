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

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Ankit Sharma");   
MODULE_DESCRIPTION("A simple linux v4l2 driver");  
MODULE_VERSION("0.1");  

static int success = 0;
struct video_device  *vdev;
struct v4l2_device    v4l2_dev;

static int __init v4l2test_init(void)
{
  int err = -ENOMEM;
  
  sprintf(v4l2_dev.name,"v4l2test");

  err = v4l2_device_register(NULL,&v4l2_dev);

  if(err)
    return err;

  vdev = video_device_alloc();

  if(vdev == NULL)
  {
    printk(KERN_ALERT "error, cannot allocate vdev");
    err = -ENOMEM; 
  }
  vdev->release = video_device_release;
  vdev->v4l2_dev = &v4l2_dev;

  err = video_register_device(vdev,VFL_TYPE_GRABBER,-1);
  
  success = 1;
  return err;
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
