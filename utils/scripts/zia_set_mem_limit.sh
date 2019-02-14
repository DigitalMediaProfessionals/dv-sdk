#!/bin/bash
# Sets upper limit on memory available for DV700 accelerator
# by setting cma option to linux kernel boot string.
# This is just maximum limit, not exclusive reservation.
# NOTE: system might refuse to apply it if value is too large.
# Use "cat /proc/meminfo | grep CmaTotal" to see the actual limit.

if [ $# != 1 ]; then
echo "USAGE: ./zia_set_mem_limit.sh MB"
exit 1
fi
mb=$1
if [ $mb -ge 0 ]; then
echo "OK" >/dev/null
else
echo "Argument MB must be >= 0"
exit 2
fi

if [ `whoami` != "root" ]; then
echo "Must be run as root"
exit 3
fi

# bootargs=earlycon clk_ignore_unused earlyprintk root=/dev/mmcblk0p2 rw rootwait cma=768MB video=DP-1:1280x720-24@60
cmdline=`fw_printenv | grep -i 'bootargs='`
if [ -n "$cmdline" ]; then
echo "OK" >/dev/null
else
echo 'Could not find u-boot variable "bootargs"'
exit 4
fi

echo "OLD bootargs: $cmdline"

args=`echo $cmdline | awk '{ print(substr($0, 10)); }'`
value=""
found=0
for arg in $args
do

if [ `echo $arg | grep -Pi ^cma= | wc -l` -eq 1 ]; then
arg=cma=""$mb"MB"
found=1
fi

if [ -n "$value" ]; then
value="$value $arg"
else
value=$arg
fi

done

if [ $found -eq 0 ]; then
value="$value cma="$mb"MB"
fi

fw_setenv bootargs $value

if [ $? -eq 0 ]; then
echo "OK" >/dev/null
else
echo "Failed to execute: fw_setenv bootargs $value"
exit 5
fi

cmdline=`fw_printenv | grep -i 'bootargs='`
echo "NEW bootargs: $cmdline"

if [ $? -eq 0 ]; then
echo "SUCCESS, reboot to apply."
echo "To see the actual value use: cat /proc/meminfo | grep CmaTotal"
else
echo "FAILED, please examine by hand u-boot environment using fw_printenv and set valiables using fw_setenv"
exit 5
fi

exit 0
