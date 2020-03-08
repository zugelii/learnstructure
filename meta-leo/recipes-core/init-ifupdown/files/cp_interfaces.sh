#!/bin/sh
file="/data/sysconf/network/interfaces"
flag=false
if [[ ! -f  $file ]]; then
  flag=true
else 
  filesize=`ls -l $file | awk '{ print $5}'`
  if [ $filesize -lt 100 ]
  then
    flag=true
  fi
fi

if [ "$flag" = true ]
then
  echo "Copying default network interface config to data partition"
  mkdir -p /data/sysconf/network
  cp /etc/network/interfaces.default /data/sysconf/network/interfaces
  if [ ! $? -eq 0 ]
  then
    echo "Copying default network interface wrong"
    time=`date +%Y-%m-%d-%H:%M:%S`
    echo "the result of copying is: " "$? "  "time: ""$time" >> /home/root/interfaces_result.txt
    rm /data/sysconf/network/interfaces
  else
    fsync
  fi  
fi


