#!/bin/sh
log_line=$(insmod ../kernel_modules/kproc.ko && sleep .4 && tail -n 1 /var/log/kern.log)

major=$(echo "$log_line" | grep "[0-9]*$" -o)

cd /dev && mknod kproc c "$major" 0
