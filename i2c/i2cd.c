#include <linux/init.h>   // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/fs.h>     // Header for the Linux file system support
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>

static struct i2c_driver *i2cd_driver;

static int __init i2cd_init(void)
{
  int result = 0;
  return result;

}

static void __exit i2cd_exit(void)
{
  
}

module_init(i2cd_init);
module_exit(i2cd_exit);

