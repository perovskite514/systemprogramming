#!/bin/sh

for i in `seq -f "%02g" 0 99`; do wget http://www.pf.is.s.u-tokyo.ac.jp/syspro/static/kadai1/1.pdf.$i; done
for i in `seq -f "%02g" 0 99`; do cat 1.pdf.$i >> 1.pdf; done 
rm -f 1.pdf.*