#include <linux/init.h>
#include <linux/version.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init ldd_init(void)
{
  printk(KERN_ALERT "Hello from ldd!\n");
  return 0;
}

 static void __exit ldd_exit(void)
 {
   printk(KERN_ALERT "Bye from ldd!\n");
 }

 module_init(ldd_init);
 module_exit(ldd_exit);
