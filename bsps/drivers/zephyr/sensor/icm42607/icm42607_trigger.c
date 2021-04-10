/*
 * Copyright (c) 2016 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/i2c.h>
#include <sys/util.h>
#include <kernel.h>
#include <drivers/sensor.h>
#include <logging/log.h>
#include "icm42607.h"
#include "icm42607_setup.h"

LOG_MODULE_DECLARE(ICM42607, CONFIG_SENSOR_LOG_LEVEL);

int icm42607_trigger_set(const struct device *dev,
			 const struct sensor_trigger *trig,
			 sensor_trigger_handler_t handler)
{
	struct icm42607_data *drv_data = dev->data;
	const struct icm42607_config *cfg = dev->config;

	if (trig->type != SENSOR_TRIG_DATA_READY
	    && trig->type != SENSOR_TRIG_TAP
	    && trig->type != SENSOR_TRIG_DOUBLE_TAP) {
		return -ENOTSUP;
	}

	gpio_pin_interrupt_configure(drv_data->gpio, cfg->int_pin,
				     GPIO_INT_DISABLE);

	if (handler == NULL) {
		icm42607_turn_off_sensor(dev);
		return 0;
	}

	if (trig->type == SENSOR_TRIG_DATA_READY) {
		drv_data->data_ready_handler = handler;
		drv_data->data_ready_trigger = *trig;
	} else if (trig->type == SENSOR_TRIG_TAP) {
		drv_data->tap_handler = handler;
		drv_data->tap_trigger = *trig;
		drv_data->tap_en = true;
	} else if (trig->type == SENSOR_TRIG_DOUBLE_TAP) {
		drv_data->double_tap_handler = handler;
		drv_data->double_tap_trigger = *trig;
		drv_data->tap_en = true;
	} else {
		return -ENOTSUP;
	}

	gpio_pin_interrupt_configure(drv_data->gpio, cfg->int_pin,
				     GPIO_INT_EDGE_TO_ACTIVE);

	icm42607_turn_on_sensor(dev);

	return 0;
}

static void icm42607_gpio_callback(const struct device *dev,
				   struct gpio_callback *cb, uint32_t pins)
{
	struct icm42607_data *drv_data =
		CONTAINER_OF(cb, struct icm42607_data, gpio_cb);
	const struct icm42607_config *cfg = drv_data->dev->config;

	ARG_UNUSED(pins);

	gpio_pin_interrupt_configure(drv_data->gpio, cfg->int_pin,
				     GPIO_INT_DISABLE);

	k_sem_give(&drv_data->gpio_sem);
}

static void icm42607_thread_cb(const struct device *dev)
{
	struct icm42607_data *drv_data = dev->data;
	const struct icm42607_config *cfg = dev->config;
	/**
	if (drv_data->data_ready_handler != NULL) {
		drv_data->data_ready_handler(dev,
					     &drv_data->data_ready_trigger);
	}
	**/
	sensor_sample_fetch(dev);

	/**
	if (drv_data->tap_handler != NULL ||
	    drv_data->double_tap_handler != NULL) {
		icm42607_tap_fetch(dev);
	}**/

	gpio_pin_interrupt_configure(drv_data->gpio, cfg->int_pin,
				     GPIO_INT_EDGE_TO_ACTIVE);
}

static void icm42607_thread(int dev_ptr, int unused)
{
	const struct device *dev = INT_TO_POINTER(dev_ptr);
	struct icm42607_data *drv_data = dev->data;

	ARG_UNUSED(unused);

	while (1) {
		k_sem_take(&drv_data->gpio_sem, K_FOREVER);
		icm42607_thread_cb(dev);
	}
}

int icm42607_init_interrupt(const struct device *dev)
{
	struct icm42607_data *drv_data = dev->data;
	const struct icm42607_config *cfg = dev->config;
	int result = 0;

	/* setup data ready gpio interrupt */
	drv_data->gpio = device_get_binding(cfg->int_label);
	if (drv_data->gpio == NULL) {
		LOG_ERR("Failed to get pointer to %s device",
			cfg->int_label);
		return -ENODEV;
	}

	drv_data->dev = dev;

	gpio_pin_configure(drv_data->gpio, cfg->int_pin,
			   GPIO_INPUT | cfg->int_flags);

	gpio_init_callback(&drv_data->gpio_cb,
			   icm42607_gpio_callback,
			   BIT(cfg->int_pin));

	result = gpio_add_callback(drv_data->gpio, &drv_data->gpio_cb);
	if (result < 0) {
		LOG_ERR("Failed to set gpio callback");
		return result;
	}

	k_sem_init(&drv_data->gpio_sem, 0, K_SEM_MAX_LIMIT);

	k_thread_create(&drv_data->thread, drv_data->thread_stack,
			CONFIG_ICM42607_THREAD_STACK_SIZE,
			(k_thread_entry_t)icm42607_thread, dev,
			0, NULL, K_PRIO_COOP(CONFIG_ICM42607_THREAD_PRIORITY),
			0, K_NO_WAIT);

	gpio_pin_interrupt_configure(drv_data->gpio, cfg->int_pin,
				     GPIO_INT_EDGE_TO_INACTIVE);

	return 0;
}
