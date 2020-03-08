#!/bin/sh
file="interfaces"
default_file="interfaces.default"
path_file="./"
flag=false
if [[ ! -f  $file ]]; then
  flag=true
else 
  filesize=`ls -l $file | awk '{ print $5}'`
  if [ $filesize -lt 200 ]
  then
    flag=true
  fi
fi

if [ "$flag" = true ]
then
  echo "Copying default network interface config to data partition"
  mkdir -p $path_file
  cp $default_file $file
  if [ ! $? -eq 0 ]
  then
    echo "Copying default network interface wrong"
    time=`date +%Y-%m-%d-%H:%M:%S`
    echo "the result of copying is: " "$? "  "time: ""$time" >> interface_result.txt
    reboot
  fi
fi


