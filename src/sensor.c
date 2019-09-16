/*
 * sensor.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Carlos Miguens
 */

#include "sensor.h"
#include "rc/mpu.h"
#include "stdlib.h"
#include "stdio.h"

RawSensor rawSensor;

Sensors *initSetSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
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

Sensors *addSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	Sensors *sensorCursor =  malloc(sizeof(Sensors));
	initSetSensor(sensorCursor, sensorType, unit, filter, initSensor, refreshSensor, closeSensor);
	while (sensors->next != NULL)
	{
		sensors = sensors->next;
	}
	sensors->next = sensorCursor;
	return sensors->next;
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
void gyroRefresh() {
	//signalFiltering(sensors.data.gyro[Z]);
	if(0>rc_mpu_read_gyro(&rawSensor.data)){
		printf("read gyro data failed\n");
		return;
	}
}
void gyroClose() {}

void accInit() {}
void accRefresh(double *value) {
	// calculate acceleration, speed and distance walked
	if(0>rc_mpu_read_accel(&rawSensor.data)){
		printf("read accel data failed\n");
		return;
	}
	*value = rawSensor.data.accel[3];
}
void accClose() {}

void usInit() {}

void usRefresh() {


}

void usClose() {}

void tempInit()
{}

void tempRefresh()
{}

void tempClose()
{}

void batteryInit() {}
void batteryRefresh() {}
void batteryClose() {}

void jackInit() {}
void jackRefresh() {}
void jackClose() {}
