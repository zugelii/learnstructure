#!/bin/bash

if [ -d /ome ]
then
	echo "this is a dir"
else
	echo "this is a file"
fi

echo "Copying default network interface config to data partition"
time=`date +%Y-%m-%d-%H:%M:%S`
echo "the result of coping is: " "$? "  "time: ""$time" >> cp.txt
if [ ! $? -eq 0 ]
then
	echo "the result is right"
else
	echo "the result is wrong"
fi
