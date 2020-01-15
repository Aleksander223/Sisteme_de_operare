/*
  Kernel module that provides functions that return pids and ppids of all the processes
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init kproc_init(void) {
    printk(KERN_LOG "Test\n");
    return 0;
}

static int __exit krpoc_exit(void) {
    printk(KERN_LOG "Exit test\n");
}

module_init(kproc_init);
moduke_exit(kproc_exit);
