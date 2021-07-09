#!/bin/sh

for i in *.cpp; do mv $i ${i%.cpp}.cc; done
for i in `find -name "*.cc"`; do sed -i -e "s/NEET the 3rd/Kotaro Shimamura/g" $i; done
for i in `find -name "*.cc"`; do sed -i -e "s/neet3@example.com/perovskite@g.ecc.u-tokyo.ac.jp/g" $i; done
for i in `find -name "*.cc"`; do sed -i -e "s/\s*$//" $i; done
