/*
 * sensor.h
 *
 *  Created on: Sep 8, 2019
 *      Author: karl
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "time.h"
#include "utils.h"
#include "rc/mpu.h"

#define I2C_BUS 2
#define GPIO_INT_PIN_CHIP 3
#define GPIO_INT_PIN_PIN  21
#define SENSORS_REFRESH_HZ 100
#define VOLTAGE_DISCONNECT	1 // Threshold for detecting disconnected battery


typedef enum {
	GYRO, ACC, US, TEMP, BATTERY, JACK, COMPASS
} SensorType;


typedef struct {
	double batt_voltage;	// 2S pack voltage on JST XH 2S balance connector
	double jack_voltage;	// could be dc power supply or another battery
} Power;

typedef struct {
	int sur;
	float x, y, z;
} Position;

void sensorsInit();
void sensorsRefresh();
void sensorsShutdown();
double getSensorValue(SensorType sensor);
char *getTelemetryReport();


#endif /* SENSOR_H_ */
