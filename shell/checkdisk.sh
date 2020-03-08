#!/bin/sh
#check the /dev/mmcblk0p7 whether is mount

if  ! test -d /run/media/mmcblk0p7 
then
	#mkfs.ext4 /dev/mmcblk0p7
	echo "/dev/mmcblk0p7 was not formatted" > /home/root/re.txt
	
	#reboot
else
	echo "the p7 has been mounted"  > /home/root/re.txt
fi
