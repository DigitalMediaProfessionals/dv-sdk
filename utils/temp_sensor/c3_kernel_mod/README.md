# How to Calculate Tempature
Value from `/sys/class/hwmon/hwmon*[0-9]+/temp*` does not show tempature.
To get tempature, we need convert the value to tempature by the following
equation.

```
tempature = <the value> / 65536.0 / 0x00196342531f - 280.2309f
```
