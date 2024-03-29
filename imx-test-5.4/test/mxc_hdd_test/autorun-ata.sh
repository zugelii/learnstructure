#!/bin/bash

#   On imx31ads board, turn off buzzer (set SW2-2 to OFF).

source /unit_tests/test-utils.sh

#
# Exit status is 0 for PASS, nonzero for FAIL
#
STATUS=0

run_hdd_case()
{
	# Generate Test data
	dd if=/dev/urandom of=/root/hdd_data bs=512 count=10240

	dd if=/root/hdd_data of=/dev/sda bs=512 count=10240
	dd if=/dev/sda of=/root/hdd_data1 bs=512 count=10240

	cmp /root/hdd_data1 /root/hdd_data

	if [ "$?" = 0 ]; then
		printf "HDD test passes \n\n"
		rm /root/hdd_data
		rm /root/hdd_data1
	else
		STATUS=1
		printf "HDD test fails \n\n"
	fi
}

echo "$(platform)"
modprobe pata_fsl
sleep 5

check_devnode "/dev/sda"

if [ "$STATUS" = 0 ]; then
	run_hdd_case
fi

print_status
exit $STATUS
