/*
  Kernel module that provides functions that return pids and ppids of all the processes
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

#define DEVICE_NAME "kproc"

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);


static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

bool hasBeenRead = false;

char procs[100000];

static int major;

int kproc_print(void) {
    struct task_struct* tasks;

    size_t nprocs = 0;

    for_each_process(tasks) {
        // pr_info("== %s [%d]\n", tasks->comm, tasks->pid);
        char tmp[256];
        sprintf(tmp, "%d %d %s %li %d\n", tasks->pid, tasks->real_parent->pid, tasks->comm, tasks->state, tasks->tgid);
        strcat(procs, tmp);
        ++nprocs;
    }

    return strlen(procs);

    // printk("== Number of processes: %zu\n", nprocs);
}

static int __init kproc_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Kproc load failed\n");
        return major;
    }

    printk("Init Kproc... %d\n", major);

    // kproc_print();
    return 0;
}

static void __exit kproc_exit(void) {
    printk("EXit Kproc..\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Kproc opened\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    printk(KERN_INFO "KProc is read-only\n");
    return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Kproc closed\n");
    hasBeenRead = false;
    procs[0] = '\0';
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    if(!hasBeenRead) {
        int errors = 0;

        int message_len = kproc_print();

        errors = copy_to_user(buffer, procs, message_len);

        hasBeenRead = true;

        return errors == 0 ? message_len : -EFAULT;
    }

    return 0;

}

module_init(kproc_init);
module_exit(kproc_exit);
