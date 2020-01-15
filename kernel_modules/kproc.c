/*
  Kernel module that provides functions that return pids and ppids of all the processes
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init kproc_init(void) {
    printk("Test\n");
    return 0;
}

static void __exit kproc_exit(void) {
    printk("Exit test\n");
}

module_init(kproc_init);
module_exit(kproc_exit);
