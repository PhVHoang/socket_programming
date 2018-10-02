#!/bin/bash
DIR=$(pwd)
x="/"

for x in `ls -d `
do
    if [ $x != "." ]; then
	rm -rf $x
    fi
done

for x in "$DIR"/*
do
    if [ $x == "$DIR/config_server.dat" ]; then
	rm $x
    fi
done

for x in "$DIR"/*
do
    if [ $x == "$DIR/history.log" ]; then
	rm $x
    fi
done
