#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init
hello_init(void)
{
  printk(KERN_ALERT "driver loaded\n");
  printk(KERN_INFO "hello world\n");
  return 0;
}

static void __exit
hello_exit(void)
{
  printk(KERN_INFO "goodbye world\n");
  printk(KERN_ALERT "driver unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
