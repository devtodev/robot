/*
 * sensor.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Carlos Miguens
 */

#include "sensor.h"
#include "stdlib.h"
#include "stdio.h"

RawSensor rawSensor;

Sensors *addSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	Sensors *sensorCursor =  malloc(sizeof(Sensors));
	initSensor(sensorCursor, sensorType, unit, filter, initSensor, refreshSensor, closeSensor);
	while (sensors->next != NULL)
	{
		sensors = sensors->next;
	}
	sensors->next = sensorCursor;
	return sensors->next;
}

Sensors *initSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	sensors->type = sensorType;
	sensors->filter = filter;
	sensors->score = 0;
	sensors->value = 0;
	sensors->unit = unit;
	sensors->start = NULL;
	sensors->end = NULL;
	sensors->refreshSensor = refreshSensor;
	sensors->initSensor = initSensor;
	sensors->closeSensor = closeSensor;
	sensors->next = NULL;
	return sensors;
}

void gyroInit() {
	rc_mpu_config_t conf = rc_mpu_default_config();
	conf.i2c_bus = I2C_BUS;
	conf.show_warnings = 0; // zero to enable
	conf.dmp_sample_rate = SAMPLE_RATE_HZ;
	conf.orient = ORIENTATION_Z_UP;
	// if gyro isn't calibrated, run the calibration routine
	if(!rc_mpu_is_gyro_calibrated())
	{
		printf("Gyro not calibrated, automatically starting calibration routine\n");
		printf("Let your MiP sit still on a firm surface\n");
		rc_mpu_calibrate_gyro_routine(conf);
	}

	if(rc_mpu_initialize(&rawSensor.data, conf)){
		printf("rc_mpu_initialize_failed\n");
		return;
	}
}
void gyroRefresh() {}
void gyroClose() {}

void accInit() {}
void accRefresh() {}
void accClose() {}

void usInit() {}
void usRefresh() {}
void usClose() {}

void batteryInit() {}
void batteryRefresh() {}
void batteryClose() {}

void jackInit() {}
void jackRefresh() {}
void jackClose() {}
