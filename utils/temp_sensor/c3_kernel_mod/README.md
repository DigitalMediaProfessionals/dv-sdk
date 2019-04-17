# How to Get Tempature

1. Build this kernel module by `make` here.
2. Install the module by `sudo insmod zia_c3_temp_mon.ko`
3. Run `dv-sdk/utils/scripts/show_temp.sh`

# How to Calculate Tempature
Value from `/sys/class/hwmon/hwmon*[0-9]+/temp*` does not show tempature.
To get tempature, we need convert the value to tempature by the following
equation.

```
tempature = <the value> / 65536.0 / 0.00196342531f - 280.2309f
```

# Terminology
Please see Xilinx Documentation labaled **WP482**
