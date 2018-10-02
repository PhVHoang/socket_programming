#!/bin/bash
DIR=$(pwd)
x="/"
for x in "$DIR"/*.o
do
    if [ $x != "$DIR/*.o" ]; then
	rm "$x"
    fi
done

for x in "$DIR"/*.a
do
    if [ $x != "$DIR/*.a" ]; then
	rm "$x"
    fi
done

for x in "$DIR"/*~
do
    if [ $x != "$DIR/*~" ]; then
	rm "$x"
    fi
done
