#!/bin/bash
DIR=$(pwd)
x="/"
for x in "$DIR"/*.exec
do
    if [ $x != "$DIR/*.exec" ]; then
	rm "$x"
    fi
done
