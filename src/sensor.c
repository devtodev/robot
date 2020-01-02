/*
 * sensor.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Carlos Miguens
 */

#include "sensor.h"
#include "rc/mpu.h"
#include "rc/adc.h"
#include "rc/time.h"
#include "rc/start_stop.h"
#include "stdlib.h"
#include "stdio.h"
#include "json.h"

RawSensor rawSensor;
Sensors sensors;
char *buffer;
Power power;

Sensors *initSetSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	buffer = malloc(4096);
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

Sensors *setDefaultSensors()
{
	initSetSensor(&sensors, GYRO, DEG, HIGH_PASS, gyroInit, gyroRefresh, gyroClose);
	addSensor(&sensors, ACC, M2S, HIGH_PASS, accInit, accRefresh, accClose);
	addSensor(&sensors, US, METER, LOW_PASS, usInit, usRefresh, usClose);
	addSensor(&sensors, TEMP, C, LOW_PASS, tempInit, tempRefresh, tempClose);
	addSensor(&sensors, BATTERY, VOLT, LOW_PASS, batteryInit, batteryRefresh, batteryClose);
	addSensor(&sensors, JACK, VOLT, LOW_PASS, jackInit, jackRefresh, jackClose);
	sensorsInit(&sensors);
	return &sensors;
}

void* __sensor_manager(__attribute__ ((unused)) void* ptr)
{
	if (setDefaultSensors() == NULL)
	{
		printf("Error to sensors initialize\n");
	}

	while(rc_get_state()!=EXITING)
	{
		sensorsRefresh(&sensors);
		rc_usleep(1000000 / SENSORS_REFRESH_HZ);
	}
	sensorsShutdown(&sensors);
	return NULL;
}


void sensorsInit(Sensors *cursor)
{
	while (cursor != NULL) {
		cursor->initSensor();
		cursor = cursor->next;
	}
}

void sensorsRefresh(Sensors *cursor)
{
	while (cursor != NULL) {
		cursor->refreshSensor(&cursor->value);
		//printf("%d) %4.1f \n", cursor->type, cursor->value);
		cursor = cursor->next;
	}
}

void sensorsShutdown(Sensors *cursor)
{
	while (cursor != NULL) {
		cursor->initSensor();
		cursor = cursor->next;
	}
}

char *getTelemetryReport()
{
	jsonBegin(buffer);
	jsonKeyDouble("temperature\0", rawSensor.data.temp, buffer);
	jsonObjBegin("accel\0", buffer);
	jsonKeyFloat("x\0", rawSensor.data.accel[0], buffer);
	jsonKeyFloat("y\0", rawSensor.data.accel[1], buffer);
	jsonKeyFloat("z\0", rawSensor.data.accel[2], buffer);
	jsonObjEnd(buffer);
	jsonObjBegin("gyro\0", buffer);
	jsonKeyDouble("x\0", rawSensor.data.compass_heading*RAD_TO_DEG, buffer);
	jsonKeyDouble("y\0", rawSensor.data.compass_heading_raw*RAD_TO_DEG, buffer);
	jsonKeyInt("z\0", (int) rawSensor.data.gyro[2], buffer);
	jsonObjEnd(buffer);
	jsonKeyDouble("battery\0", power.batt_voltage, buffer);
	jsonKeyDouble("jack\0", power.jack_voltage, buffer);
	jsonEnd(buffer);
	return buffer;
}

Sensors* getSensor(Sensors *cursor, SensorType type)
{
	while(cursor != NULL)
	{
		if (cursor->type == type)
		{
			return cursor;
		}
	}
	return NULL;
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
	conf.gpio_interrupt_pin_chip = GPIO_INT_PIN_CHIP;
	conf.gpio_interrupt_pin = GPIO_INT_PIN_PIN;
	conf.show_warnings = 0; // zero to enable
	conf.enable_magnetometer = 1;
	conf.dmp_fetch_accel_gyro=1;
	conf.dmp_fetch_accel_gyro=1;
	// now set up the imu for dmp interrupt operation
	if(rc_mpu_initialize_dmp(&rawSensor.data, conf)){
		printf("rc_mpu_initialize_failed\n");
		return;
	}
}
void gyroRefresh(double *value) {
	//signalFiltering(sensors.data.gyro[Z]);
	*value = rawSensor.data.compass_heading*RAD_TO_DEG;
}
void gyroClose() {}

void accInit() {}

void accRefresh(double *value) {
	// calculate acceleration, speed and distance walked
	*value = rawSensor.data.accel[0];
}
void accClose() {}

void usInit() {}

void usRefresh(double *value) {
	*value = 0;
}

void usClose() {}

void tempInit()
{}

void tempRefresh(double *value)
{
	*value = rawSensor.data.temp;
}

void tempClose()
{}

void batteryInit() {
    if ((-1 == rc_enable_signal_handler()) ||
    	(-1 == rc_adc_init()))
    {
            fprintf(stderr,"ERROR: failed to run rc_init_adc()\n");
            return;
    }
}

void batteryRefresh(double *value) {
	*value = rc_adc_batt();
	// read in the voltage of the 2S pack and DC jack
	power.batt_voltage = rc_adc_batt();

	// check if a pack is on the 2S balance connector
	if(power.batt_voltage<VOLTAGE_DISCONNECT){
		power.batt_voltage = 0;
	}
}

void batteryClose() {
	rc_adc_cleanup();
	rc_mpu_power_off();
}

void jackInit() {
    if ((-1 == rc_enable_signal_handler()) ||
    	(-1 == rc_adc_init()))
    {
            fprintf(stderr,"ERROR: failed to run rc_init_adc()\n");
    }
}
void jackRefresh(double *value) {
	*value = rc_adc_dc_jack();
	power.jack_voltage = rc_adc_dc_jack();

	if(power.jack_voltage<VOLTAGE_DISCONNECT){
		power.jack_voltage = 0;
	}

}
void jackClose() {
	rc_adc_cleanup();
	rc_mpu_power_off();
}
