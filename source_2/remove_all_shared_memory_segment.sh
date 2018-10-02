#!/bin/sh
for i in $(ipcs -m | awk '{ print $2 }' | sed 1,2d);
do
    echo "ipcrm -m $i"
    sudo ipcrm -m $i
done
