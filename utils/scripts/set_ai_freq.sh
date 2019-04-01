#!/bin/bash

freq=$1
max_freq=316
min_freq=50

sysfs_conv_freq=/sys/class/dmp_dv/dv_conv/conv_freq

# check arg
expr $freq + 1 >/dev/null 2>&1
RET=$?
if [[ -z "$freq" ]] || [[ $RET -eq 2 ]] || [[ $RET -eq 3 ]]; then
	echo '[ERROR] Please input a natural number' 1>&2
	exit -1
fi

if [[ $freq -gt $max_freq ]] || [[ $freq -lt $min_freq ]]; then
	echo '[ERROR] Please input a valid frequency' 1>&2
	echo '[ERROR]'" The valid range is from $min_freq to $max_freq" 1>&2
	exit -1
fi

sudo bash -c "echo $freq >$sysfs_conv_freq"

echo "Frequency of AI Processor is set as $(cat $sysfs_conv_freq) MHz"
