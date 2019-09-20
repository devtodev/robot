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
#define SENSORS_REFRESH_HZ 100
#define VOLTAGE_DISCONNECT	1 // Threshold for detecting disconnected battery


typedef enum {
	GYRO, ACC, US, TEMP, BATTERY, JACK, METER
} SensorType;

typedef enum {
	RAW, M2S, DEG, RAD, VOLT, C
} UnitType;

typedef enum {
	LOW_PASS, HIGH_PASS, INTEGRATOR, LP_BUTTER, HP_BUTTER
} FilterType;

typedef struct _Sensor {
	SensorType type;
	double value;
	UnitType unit;
	FilterType filter;
	time_t *start;
	time_t *end;
	int score;
	void (*initSensor)();
	void (*refreshSensor)();
	void (*closeSensor)();
	rc_mpu_config_t *mpu_config;
	struct _Sensor *next;
} Sensors;


typedef enum g_mode_t{
	G_MODE_RAD, G_MODE_DEG, G_MODE_RAW
} g_mode_t;

typedef enum a_mode_t{
	A_MODE_MS2, A_MODE_G, A_MODE_RAW
} a_mode_t;


typedef struct {
	rc_mpu_data_t data; 	// struct to hold new data
	g_mode_t g_mode; 		// gyro default to degree mode.
	a_mode_t a_mode; 		// accel default to m/s^2
} RawSensor;

typedef struct {
	double batt_voltage;	// 2S pack voltage on JST XH 2S balance connector
	double jack_voltage;	// could be dc power supply or another battery
} Power;

Sensors *addSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)());

Sensors *initSetSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)());


void sensorsInit(Sensors *cursor);
void sensorsRefresh(Sensors *cursor);
void sensorsShutdown(Sensors *cursor);
Sensors *setDefaultSensors();
Sensors* getSensor(Sensors *cursor, SensorType type);
char *getTelemetryReport();
void* __sensor_manager(__attribute__ ((unused)) void* ptr);

void gyroInit();
void gyroRefresh(double *value);
void gyroClose();
void accInit();
void accRefresh(double *value);
void accClose();
void usInit();
void usRefresh(double *value);
void usClose();
void tempInit();
void tempRefresh(double *value);
void tempClose();
void batteryInit();
void batteryRefresh(double *value);
void batteryClose();
void jackInit();
void jackRefresh(double *value);
void jackClose();

#endif /* SENSOR_H_ */
