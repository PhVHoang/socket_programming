#!/bin/bash
DIR=$(pwd)
x="/"

clear
echo "Library files:"
for x in "$DIR"/*.h
do
    if [ $x != "$DIR/*.h" ]; then
	echo -e "\t + ${x:${#DIR}+1}" 
    fi
done

echo "Header files:"
for x in "$DIR"/*.h
do
    if [ $x != "$DIR/*.h" ]; then
	echo -e "\t + ${x:${#DIR}+1}" 
    fi
done

echo "C Source code files:"
for x in "$DIR"/*.c
do
    if [ $x != "$DIR/*.c" ]; then
	echo -e "\t + ${x:${#DIR}+1}" 
    fi
done


echo "Object files:"
for x in "$DIR"/*.o
do
    if [ $x != "$DIR/*.o" ]; then
	echo -e "\t + ${x:${#DIR}+1}" 
    fi
done

echo "Excute files:"
tput setaf 2
for x in "$DIR"/*.exec
do
    if [ $x != "$DIR/*.exec" ]; then
	echo -e "\t + ${x:${#DIR}+1}" 
    fi
done
tput sgr0

echo "Misc files:"
for x in "$DIR"/*
do
    if [ $x != "$DIR/*" ]; then
	if [ ${x: -2} != ".c" ] && [ ${x: -2} != ".o" ] && [ ${x: -2} != ".h" ] && [ ${x: -5} != ".exec" ]; then
	    echo -e "\t + ${x:${#DIR}+1}"  
	fi	 
    fi
done
