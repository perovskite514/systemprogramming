#!/bin/sh
strace /bin/cat /proc/meminfo 1> cat.txt
strace /bin/cat /proc/meminfo 2> strace.txt
strace /bin/cat /proc/meminfo > all.txt 2>&1