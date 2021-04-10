/*
 * Copyright (c) 2020 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT invensense_icm42607

#include <drivers/spi.h>
#include <init.h>
#include <sys/byteorder.h>
#include <drivers/sensor.h>
#include <logging/log.h>

#include "icm42607.h"
#include "icm42607_reg.h"
#include "icm42607_setup.h"
#include "icm42607_spi.h"

struct k_mutex icm42607_mutex;

LOG_MODULE_REGISTER(ICM42607, CONFIG_SENSOR_LOG_LEVEL);

static const uint16_t icm42607_gyro_sensitivity_x10[] = {
	1310, 655, 328, 164
};

/* see "Accelerometer Measurements" section from register map description */
static void icm42607_convert_accel(struct sensor_value *val,
				   int16_t raw_val,
				   uint16_t sensitivity_shift)
{
	/**
	int64_t conv_val;

	conv_val = ((int64_t)raw_val * SENSOR_G) >> sensitivity_shift;
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
	**/

	/*put acc/gyro  register data*/
	val->val1 = raw_val;
	val->val2 = 0;
}

/* see "Gyroscope Measurements" section from register map description */
static void icm42607_convert_gyro(struct sensor_value *val,
				  int16_t raw_val,
				  uint16_t sensitivity_x10)
{
	/**
	int64_t conv_val;

	conv_val = ((int64_t)raw_val * SENSOR_PI * 10) /
		   (sensitivity_x10 * 180U);
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
	**/
	/*put acc/gyro  register data*/
	val->val1 = raw_val;
	val->val2 = 0;
}

/* see "Temperature Measurement" section from register map description */
static inline void icm42607_convert_temp(struct sensor_value *val,
					 int16_t raw_val)
{
	val->val1 = (((int64_t)raw_val * 100) / 207) + 25;
	val->val2 = ((((int64_t)raw_val * 100) % 207) * 1000000) / 207;

	if (val->val2 < 0) {
		val->val1--;
		val->val2 += 1000000;
	} else if (val->val2 >= 1000000) {
		val->val1++;
		val->val2 -= 1000000;
	}
}

static int icm42607_channel_get(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val)
{
	const struct icm42607_data *drv_data = dev->data;

	switch (chan) {
	case SENSOR_CHAN_ACCEL_XYZ:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_accel(val, drv_data->accel_x,
				       drv_data->accel_sensitivity_shift);
			icm42607_convert_accel(val + 1, drv_data->accel_y,
				       drv_data->accel_sensitivity_shift);
			icm42607_convert_accel(val + 2, drv_data->accel_z,
				       drv_data->accel_sensitivity_shift);
			k_mutex_unlock(&(icm42607_mutex));		
		}
		
		break;
	case SENSOR_CHAN_ACCEL_X:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_accel(val, drv_data->accel_x,
				       drv_data->accel_sensitivity_shift);
			k_mutex_unlock(&(icm42607_mutex));
		}
		
		break;
	case SENSOR_CHAN_ACCEL_Y:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_accel(val, drv_data->accel_y,
				       drv_data->accel_sensitivity_shift);
			k_mutex_unlock(&(icm42607_mutex));
		}
		
		break;
	case SENSOR_CHAN_ACCEL_Z:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_accel(val, drv_data->accel_z,
						drv_data->accel_sensitivity_shift);
			k_mutex_unlock(&(icm42607_mutex));		   
		}
		
		break;
	case SENSOR_CHAN_GYRO_XYZ:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_gyro(val, drv_data->gyro_x,
				      drv_data->gyro_sensitivity_x10);
			icm42607_convert_gyro(val + 1, drv_data->gyro_y,
						drv_data->gyro_sensitivity_x10);
			icm42607_convert_gyro(val + 2, drv_data->gyro_z,
						drv_data->gyro_sensitivity_x10);
			k_mutex_unlock(&(icm42607_mutex));			
		}	
		
		break;
	case SENSOR_CHAN_GYRO_X:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_gyro(val, drv_data->gyro_x,
				      drv_data->gyro_sensitivity_x10);
			k_mutex_unlock(&(icm42607_mutex));
		}
		
		break;
	case SENSOR_CHAN_GYRO_Y:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_gyro(val, drv_data->gyro_y,
				      drv_data->gyro_sensitivity_x10);
			k_mutex_unlock(&(icm42607_mutex));		  
		}
		
		break;
	case SENSOR_CHAN_GYRO_Z:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_gyro(val, drv_data->gyro_z,
				      drv_data->gyro_sensitivity_x10);
			k_mutex_unlock(&(icm42607_mutex));	
		}
		
		break;
	case SENSOR_CHAN_DIE_TEMP:

		if (k_mutex_lock(&(icm42607_mutex), K_FOREVER) == 0) {
			icm42607_convert_temp(val, drv_data->temp);
			k_mutex_unlock(&(icm42607_mutex));
		}
		
		break;
	default:
		return -ENOTSUP;
	}

	return 0;
}

int icm42607_tap_fetch(const struct device *dev)
{
	int result = 0;
	struct icm42607_data *drv_data = dev->data;

	if (drv_data->tap_en &&
	    (drv_data->tap_handler || drv_data->double_tap_handler)) {
		result = inv_spi_read(REG_INT_STATUS3, drv_data->fifo_data, 1);
		if (drv_data->fifo_data[0] & BIT_INT_STATUS_TAP_DET) {
			result = inv_spi_read(REG_APEX_DATA4,
					      drv_data->fifo_data, 1);
			if (drv_data->fifo_data[0] & APEX_TAP) {
				if (drv_data->tap_trigger.type ==
				    SENSOR_TRIG_TAP) {
					LOG_DBG("Single Tap detected");
					drv_data->tap_handler(dev
					      , &drv_data->tap_trigger);
				} else {
					LOG_ERR("Trigger type is mismatched");
				}
			} else if (drv_data->fifo_data[0] & APEX_DOUBLE_TAP) {
				if (drv_data->double_tap_trigger.type ==
				    SENSOR_TRIG_DOUBLE_TAP) {
					LOG_DBG("Double Tap detected");
					drv_data->double_tap_handler(dev
						     , &drv_data->tap_trigger);
				} else {
					LOG_ERR("Trigger type is mismatched");
				}
			} else {
				LOG_DBG("Not supported tap event");
			}
		}
	}

	return 0;
}

static int icm42607_sample_fetch(const struct device *dev,
				 enum sensor_channel chan)
{
	int result = 0;
	uint16_t fifo_count = 0;
	struct icm42607_data *drv_data = dev->data;


	inv_spi_read(0x3d, drv_data->fifo_data, 2);
	fifo_count = (drv_data->fifo_data[0] << 8) 
		+ (drv_data->fifo_data[1]);

	result = inv_spi_read(0x39, drv_data->fifo_data, 1);
	if (drv_data->fifo_data[0] & BIT_INT_STATUS_DRDY){
		uint8_t axis_data[6] = {0};
		uint8_t gyro_data[6] = {0};

		inv_spi_read(0xB,&axis_data[0],1);
		inv_spi_read(0xC,&axis_data[1],1);
		inv_spi_read(0xD,&axis_data[2],1);
		inv_spi_read(0xE,&axis_data[3],1);
		inv_spi_read(0xF,&axis_data[4],1);
		inv_spi_read(0x10,&axis_data[5],1);

		drv_data->accel_x = (axis_data[0] << 8)|axis_data[1];
		drv_data->accel_y = (axis_data[2] << 8)|axis_data[3];
		drv_data->accel_z = (axis_data[4] << 8)|axis_data[6];

		inv_spi_read(0x11, gyro_data,6);

		drv_data->gyro_x = (gyro_data[0] << 8)|gyro_data[1];
		drv_data->gyro_y = (gyro_data[2] << 8)|gyro_data[3];
		drv_data->gyro_z = (gyro_data[4] << 8)|gyro_data[6];

		LOG_DBG("acc_x : %d, acc_y : %d, acc_y : %d\n", 
		 	drv_data->accel_x, drv_data->accel_y, drv_data->accel_z);
	    LOG_DBG("gyro_x : %d, gyro_y : %d, gyro_y : %d\n", 
		 	drv_data->gyro_x, drv_data->gyro_y, drv_data->gyro_z);
	}

	

	/* Read INT_STATUS (0x45) and FIFO_COUNTH(0x46), FIFO_COUNTL(0x47) */
	
	if (0){

		inv_spi_read(0x3d, drv_data->fifo_data, 2);
		fifo_count = (drv_data->fifo_data[0] << 8)
			+ (drv_data->fifo_data[1]);

		result = inv_spi_read(0x3f, drv_data->fifo_data,
						fifo_count);

		/* FIFO Data structure
			* Packet 1 : FIFO Header(1), AccelX(2), AccelY(2),
			*            AccelZ(2), Temperature(1)
			* Packet 2 : FIFO Header(1), GyroX(2), GyroY(2),
			*            GyroZ(2), Temperature(1)
			* Packet 3 : FIFO Header(1), AccelX(2), AccelY(2), AccelZ(2),
			*            GyroX(2), GyroY(2), GyroZ(2), Temperature(1)
			*/
		if (drv_data->fifo_data[0] & BIT_FIFO_HEAD_ACCEL) {
			/* Check empty values */
			if (!(drv_data->fifo_data[1] == FIFO_ACCEL0_RESET_VALUE
					&& drv_data->fifo_data[2] ==
					FIFO_ACCEL1_RESET_VALUE)) {
				drv_data->accel_x =
					(drv_data->fifo_data[1] << 8)
					+ (drv_data->fifo_data[2]);
				drv_data->accel_y =
					(drv_data->fifo_data[3] << 8)
					+ (drv_data->fifo_data[4]);
				drv_data->accel_z =
					(drv_data->fifo_data[5] << 8)
					+ (drv_data->fifo_data[6]);
			}
			if (!(drv_data->fifo_data[0] & BIT_FIFO_HEAD_GYRO)) {
				drv_data->temp =
					(int16_t)(drv_data->fifo_data[7]);
			} else {
				if (!(drv_data->fifo_data[7] ==
						FIFO_GYRO0_RESET_VALUE &&
						drv_data->fifo_data[8] ==
						FIFO_GYRO1_RESET_VALUE)) {
					drv_data->gyro_x =
						(drv_data->fifo_data[7] << 8)
						+ (drv_data->fifo_data[8]);
					drv_data->gyro_y =
						(drv_data->fifo_data[9] << 8)
						+ (drv_data->fifo_data[10]);
					drv_data->gyro_z =
						(drv_data->fifo_data[11] << 8)
						+ (drv_data->fifo_data[12]);
				}
				drv_data->temp =
					(int16_t)(drv_data->fifo_data[13]);
			}
		} else {
			if (drv_data->fifo_data[0] & BIT_FIFO_HEAD_GYRO) {
				if (!(drv_data->fifo_data[1] ==
						FIFO_GYRO0_RESET_VALUE &&
						drv_data->fifo_data[2] ==
						FIFO_GYRO1_RESET_VALUE)) {
					drv_data->gyro_x =
						(drv_data->fifo_data[1] << 8)
						+ (drv_data->fifo_data[2]);
					drv_data->gyro_y =
						(drv_data->fifo_data[3] << 8)
						+ (drv_data->fifo_data[4]);
					drv_data->gyro_z =
						(drv_data->fifo_data[5] << 8)
						+ (drv_data->fifo_data[6]);
				}
				drv_data->temp =
					(int16_t)(drv_data->fifo_data[7]);
			}
		}
	}

	return 0;
}

static int icm42607_attr_set(const struct device *dev,
			     enum sensor_channel chan,
			     enum sensor_attribute attr,
			     const struct sensor_value *val)
{
	struct icm42607_data *drv_data = dev->data;

	__ASSERT_NO_MSG(val != NULL);

	switch (chan) {
	case SENSOR_CHAN_ACCEL_X:
	case SENSOR_CHAN_ACCEL_Y:
	case SENSOR_CHAN_ACCEL_Z:
	case SENSOR_CHAN_ACCEL_XYZ:
		if (attr == SENSOR_ATTR_SAMPLING_FREQUENCY) {
			if (val->val1 > 8000 || val->val1 < 1) {
				LOG_ERR("Incorrect sampling value");
				return -EINVAL;
			} else {
				drv_data->accel_hz = val->val1;
			}
		} else if (attr == SENSOR_ATTR_FULL_SCALE) {
			if (val->val1 < ACCEL_FS_16G ||
			    val->val1 > ACCEL_FS_2G) {
				LOG_ERR("Incorrect fullscale value");
				return -EINVAL;
			} else {
				drv_data->accel_sf = val->val1;
			}
		} else {
			LOG_ERR("Not supported ATTR");
			return -ENOTSUP;
		}

		break;
	case SENSOR_CHAN_GYRO_X:
	case SENSOR_CHAN_GYRO_Y:
	case SENSOR_CHAN_GYRO_Z:
	case SENSOR_CHAN_GYRO_XYZ:
		if (attr == SENSOR_ATTR_SAMPLING_FREQUENCY) {
			if (val->val1 > 8000 || val->val1 < 12) {
				LOG_ERR("Incorrect sampling value");
				return -EINVAL;
			} else {
				drv_data->gyro_hz = val->val1;
			}
		} else if (attr == SENSOR_ATTR_FULL_SCALE) {
			if (val->val1 < GYRO_FS_2000DPS ||
			    val->val1 > GYRO_FS_15DPS) {
				LOG_ERR("Incorrect fullscale value");
				return -EINVAL;
			} else {
				drv_data->gyro_sf = val->val1;
			}
		} else {
			LOG_ERR("Not supported ATTR");
			return -EINVAL;
		}
		break;
	default:
		LOG_ERR("Not support");
		return -EINVAL;
	}

	return 0;
}

static int icm42607_attr_get(const struct device *dev,
			     enum sensor_channel chan,
			     enum sensor_attribute attr,
			     struct sensor_value *val)
{
	const struct icm42607_data *drv_data = dev->data;

	__ASSERT_NO_MSG(val != NULL);

	switch (chan) {
	case SENSOR_CHAN_ACCEL_X:
	case SENSOR_CHAN_ACCEL_Y:
	case SENSOR_CHAN_ACCEL_Z:
	case SENSOR_CHAN_ACCEL_XYZ:
		if (attr == SENSOR_ATTR_SAMPLING_FREQUENCY) {
			val->val1 = drv_data->accel_hz;
		} else if (attr == SENSOR_ATTR_FULL_SCALE) {
			val->val1 = drv_data->accel_sf;
		} else {
			LOG_ERR("Not supported ATTR");
			return -EINVAL;
		}

		break;
	case SENSOR_CHAN_GYRO_X:
	case SENSOR_CHAN_GYRO_Y:
	case SENSOR_CHAN_GYRO_Z:
	case SENSOR_CHAN_GYRO_XYZ:
		if (attr == SENSOR_ATTR_SAMPLING_FREQUENCY) {
			val->val1 = drv_data->gyro_hz;
		} else if (attr == SENSOR_ATTR_FULL_SCALE) {
			val->val1 = drv_data->gyro_sf;
		} else {
			LOG_ERR("Not supported ATTR");
			return -EINVAL;
		}

		break;

	default:
		LOG_ERR("Not support");
		return -EINVAL;
	}

	return 0;
}

static int icm42607_data_init(struct icm42607_data *data,
			      const struct icm42607_config *cfg)
{
	data->accel_x = 0;
	data->accel_y = 0;
	data->accel_z = 0;
	data->temp = 0;
	data->gyro_x = 0;
	data->gyro_y = 0;
	data->gyro_z = 0;
	data->accel_hz = cfg->accel_hz;
	data->gyro_hz = cfg->gyro_hz;

	data->accel_sf = cfg->accel_fs;
	data->gyro_sf = cfg->gyro_fs;

	data->tap_en = false;
	data->sensor_started = false;

	return 0;
}

static int icm42607_init(const struct device *dev)
{
	struct icm42607_data *drv_data = dev->data;
	const struct icm42607_config *cfg = dev->config;

	drv_data->spi = device_get_binding(cfg->spi_label);
	if (!drv_data->spi) {
		LOG_ERR("SPI device not exist");
		return -ENODEV;
	}

	drv_data->spi_cs.gpio_dev = device_get_binding(cfg->gpio_label);

	if (!drv_data->spi_cs.gpio_dev) {
		LOG_ERR("GPIO device not exist");
		return -ENODEV;
	}

	drv_data->spi_cs.gpio_pin = cfg->gpio_pin;
	drv_data->spi_cs.gpio_dt_flags = cfg->gpio_dt_flags;
	drv_data->spi_cs.delay = 0U;

	drv_data->spi_cfg.frequency = cfg->frequency;
	drv_data->spi_cfg.slave = cfg->slave;
	drv_data->spi_cfg.operation = (SPI_OP_MODE_MASTER | SPI_MODE_CPOL |
			SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE |
			SPI_TRANSFER_MSB);
	drv_data->spi_cfg.cs = &drv_data->spi_cs;

	k_mutex_init(&(icm42607_mutex));
	icm42607_spi_init(drv_data->spi, &drv_data->spi_cfg);
	icm42607_data_init(drv_data, cfg);
	icm42607_sensor_init(dev);

	drv_data->accel_sensitivity_shift = 14 - 3;
	drv_data->gyro_sensitivity_x10 = icm42607_gyro_sensitivity_x10[3];
	
	if (icm42607_init_interrupt(dev) < 0) {
		LOG_ERR("Failed to initialize interrupts.");
		return -EIO;
	}
	
	LOG_DBG("Initialize interrupt done");

	return 0;
}

static const struct sensor_driver_api icm42607_driver_api = {
	//.trigger_set = icm42607_trigger_set,
	.sample_fetch = icm42607_sample_fetch,
	.channel_get = icm42607_channel_get,
	.attr_set = icm42607_attr_set,
	.attr_get = icm42607_attr_get,
};
//		.frequency = DT_INST_PROP(index, spi_max_frequency),

#define ICM42607_DEFINE_CONFIG(index)					\
	static const struct icm42607_config icm42607_cfg_##index = {	\
		.spi_label = DT_INST_BUS_LABEL(index),			\
		.spi_addr = DT_INST_REG_ADDR(index),			\
		.frequency = 1000000,	\
		.slave = 0x68,			\	
		.int_label = DT_INST_GPIO_LABEL(index, int_gpios),	\
		.int_pin =  DT_INST_GPIO_PIN(index, int_gpios),			\
		.int_flags = 0,			\
		.gpio_label = DT_INST_SPI_DEV_CS_GPIOS_LABEL(index), 	\
		.gpio_pin = DT_INST_SPI_DEV_CS_GPIOS_PIN(index),	\
		.gpio_dt_flags = 1,		\
		.accel_hz = 50,			\
		.gyro_hz = 50,			\
		.accel_fs = 50,			\
		.gyro_fs = 50,			\
	}

/*
#define ICM42607_DEFINE_CONFIG(index)					\
	static const struct icm42607_config icm42607_cfg_##index = {	\
		.spi_label = DT_INST_BUS_LABEL(index),			\
		.spi_addr = DT_INST_REG_ADDR(index),			\
		.frequency = 1000000,	\
		.slave = DT_INST_REG_ADDR(index),			\
		.int_label = DT_INST_GPIO_LABEL(index, int_gpios),	\
		.int_pin =  DT_INST_GPIO_PIN(index, int_gpios),		\
		.int_flags = DT_INST_GPIO_FLAGS(index, int_gpios),	\
		.gpio_label = DT_INST_SPI_DEV_CS_GPIOS_LABEL(index),	\
		.gpio_pin = DT_INST_SPI_DEV_CS_GPIOS_PIN(index),	\
		.gpio_dt_flags = DT_INST_SPI_DEV_CS_GPIOS_FLAGS(index),	\
		.accel_hz = DT_INST_PROP(index, accel_hz),		\
		.gyro_hz = DT_INST_PROP(index, gyro_hz),		\
		.accel_fs = DT_ENUM_IDX(DT_DRV_INST(index), accel_fs),	\
		.gyro_fs = DT_ENUM_IDX(DT_DRV_INST(index), gyro_fs),	\
	}
*/

#define ICM42607_INIT(index)						\
	ICM42607_DEFINE_CONFIG(index);					\
	static struct icm42607_data icm42607_driver_##index;		\
	DEVICE_DT_INST_DEFINE(index, icm42607_init,			\
			    device_pm_control_nop,			\
			    &icm42607_driver_##index,			\
			    &icm42607_cfg_##index, POST_KERNEL,		\
			    CONFIG_SENSOR_INIT_PRIORITY,		\
			    &icm42607_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ICM42607_INIT)
