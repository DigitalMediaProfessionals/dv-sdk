/*
 *  Tempareture Monitoring Driver for ZIA C2
 *
 *  Copyright (C) 2019  Digital Media Professionals Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <asm/io.h>
#include <asm/types.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>

#define BASE 0xff204004
#define SIZE 8
#define NAME "zia_c2_tempature_monitor"

static struct device *hwmon_dev;

static ssize_t show_temp1_input(struct device *dev,
			  struct device_attribute *da, char *buf)
{
	volatile void * addr = dev_get_drvdata(dev);
	u32 val = 0;
	iowrite32(1, addr); // reset
	udelay(1);
	iowrite32(0, addr);
	udelay(1);
	iowrite32(0x10, addr); // clock enable
	msleep(1);
	iowrite32(0x00, addr); // clock disable

	val = ioread32(addr + 4);
	if ((val >> 8) & 0x1) {
		u32 temp = (val & 0xff) - 128;
		return snprintf(buf, PAGE_SIZE, "%u\n", temp * 1000);
	} else {
		return -ENODATA;
	}
}

static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp1_input, NULL, 0);

/* pointers to created device attributes */
static struct attribute *zia_c2_temp_attrs[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	NULL,
};

static const struct attribute_group zia_c2_temp_group = {
	.attrs = zia_c2_temp_attrs,
};

static const struct attribute_group *groups[] = {
	&zia_c2_temp_group,
	NULL
};

static int __init drm_init(void)
{
	void * addr;
	addr = ioremap_nocache(BASE, SIZE);
	hwmon_dev = hwmon_device_register_with_groups(NULL, NAME,
							   addr, groups);

	return 0;
}

static void __exit drm_exit(void)
{
	void * addr = dev_get_drvdata(hwmon_dev);
	iounmap(addr);
	hwmon_device_unregister(hwmon_dev);
}

module_init(drm_init);
module_exit(drm_exit);

MODULE_AUTHOR("Digital Media Professionals Inc.");
MODULE_DESCRIPTION("ZIA C2 Tempareture Monitor Driver");
MODULE_VERSION("7.0.20190313");
MODULE_LICENSE("GPL");
