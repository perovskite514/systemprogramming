#!/bin/bash

for i in `find coreutils-8.9 -name "*.c"`; do wc -l $i; done | sort -h | awk '{print$2}' > result.txt