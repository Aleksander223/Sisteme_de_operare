#!/bin/sh
c_PWD=$(pwd)

cd /dev && rm -f kproc
cd "$c_PWD" && rmmod kproc
