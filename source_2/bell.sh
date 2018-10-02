#!/bin/sh
speaker-test --frequency 500 --test sine & pid=$!;
sleep 0.4s;
kill -9 $pid;
