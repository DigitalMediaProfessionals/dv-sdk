/*
 *  Tempareture Monitoring Driver for ZIA C3
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

#define BASE 0xffa50800
#define SIZE 8
#define NAME "zia_c3_tempature_monitor"

static struct device *hwmon_dev;

#define PS	0x0
#define PL	0x1

#define LPD_CUR	(0 << 1)
#define LPD_MIN	(1 << 1)
#define LPD_MAX	(2 << 1)
#define FPD_CUR	(3 << 1)
#define FPD_MIN	(4 << 1)
#define FPD_MAX	(5 << 1)

static int REG_OFFSET[6] = { 0x000, 0x090, 0x080,
			     0x214, 0x2b4, 0x294, };
static int PSPL_OFFSET[2] = {0, 0x400};

static u32 read_temp(volatile void * base, int dev_index)
{
	volatile void *p = 0x00;
	int reg_index = dev_index >> 1;
	int pspl_index = dev_index & 0x1;
	p = base + REG_OFFSET[reg_index] + PSPL_OFFSET[pspl_index];
	return ioread32(p);
}

static ssize_t show_value(struct device *dev,
			  struct device_attribute *da, char *buf)
{
	volatile void * addr = dev_get_drvdata(dev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);

	u32 temp = read_temp(addr, attr->index);
	return snprintf(buf, PAGE_SIZE, "%u\n", temp);
}

static SENSOR_DEVICE_ATTR(temp_pslpd_input, S_IRUGO, show_value, NULL,
			  PS | LPD_CUR);
static SENSOR_DEVICE_ATTR(temp_pslpd_min,   S_IRUGO, show_value, NULL,
			  PS | LPD_MIN);
static SENSOR_DEVICE_ATTR(temp_pslpd_max,   S_IRUGO, show_value, NULL,
			  PS | LPD_MAX);
static SENSOR_DEVICE_ATTR(temp_psfpd_input, S_IRUGO, show_value, NULL,
			  PS | FPD_CUR);
static SENSOR_DEVICE_ATTR(temp_psfpd_min,   S_IRUGO, show_value, NULL,
			  PS | FPD_MIN);
static SENSOR_DEVICE_ATTR(temp_psfpd_max,   S_IRUGO, show_value, NULL,
			  PS | FPD_MAX);
static SENSOR_DEVICE_ATTR(temp_pllpd_input, S_IRUGO, show_value, NULL,
			  PL | LPD_CUR);
static SENSOR_DEVICE_ATTR(temp_pllpd_min,   S_IRUGO, show_value, NULL,
			  PL | LPD_MIN);
static SENSOR_DEVICE_ATTR(temp_pllpd_max,   S_IRUGO, show_value, NULL,
		   	  PL | LPD_MAX);

/* pointers to created device attributes */
static struct attribute *zia_c3_temp_attrs[] = {
	&sensor_dev_attr_temp_pslpd_input.dev_attr.attr,
	&sensor_dev_attr_temp_pslpd_min.dev_attr.attr,
	&sensor_dev_attr_temp_pslpd_max.dev_attr.attr,
	&sensor_dev_attr_temp_psfpd_input.dev_attr.attr,
	&sensor_dev_attr_temp_psfpd_min.dev_attr.attr,
	&sensor_dev_attr_temp_psfpd_max.dev_attr.attr,
	&sensor_dev_attr_temp_plfpd_input.dev_attr.attr,
	&sensor_dev_attr_temp_plfpd_min.dev_attr.attr,
	&sensor_dev_attr_temp_plfpd_max.dev_attr.attr,
	NULL,
};

static const struct attribute_group zia_c3_temp_group = {
	.attrs = zia_c3_temp_attrs,
};

static const struct attribute_group *groups[] = {
	&zia_c3_temp_group,
	NULL
};

static int __init drm_init(void)
{
	unsigned long sz = 4 + PSPL_OFFSET[1] + REG_OFFSET[5];
	void * addr = ioremap_nocache(BASE, sz);
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
MODULE_DESCRIPTION("ZIA C3 Tempareture Monitor Driver");
MODULE_VERSION("7.0.20190320");
MODULE_LICENSE("GPL");
