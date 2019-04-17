#!/bin/bash

calc_temp() {
	t=$(echo "scale=10; $1 / 65536.0 / 0.0019342531 - 280.2309" | bc)
	printf "%.2f" $t
}

for hwmon_d in /sys/class/hwmon/hwmon*
do
	if [[ "$(cat $hwmon_d/name)" == "zia_c3_tempature_monitor" ]]
	then
		echo -n "[Full Power Domain],"
		echo -n "$(calc_temp $(cat $hwmon_d/temp_psfpd_input)),"

		echo -n "[Low Power Domain],"
		echo -n "$(calc_temp $(cat $hwmon_d/temp_pslpd_input)),"

		echo -n "[PL Domain],"
		echo -n "$(calc_temp $(cat $hwmon_d/temp_pllpd_input))"
		echo

		exit 0
	fi
done

echo "[ERROR] zia_c3_tempature_monitor hwmon is not found" >&2
exit -1
