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

typedef enum {
	ACC, GYRO, US, METER, BATTERY, JACK
} SensorType;

typedef enum {
	RAW, M2S, DEG, RAD, VOLT
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
	// sensor array->next
	struct _Sensor *next;
} Sensors;

Sensors *addSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)());

Sensors *initSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)());

void gyroInit();
void gyroRefresh();
void gyroClose();
void accInit();
void accRefresh();
void accClose();
void usInit();
void usRefresh();
void usClose();
void batteryInit();
void batteryRefresh();
void batteryClose();
void jackInit();
void jackRefresh();
void jackClose();

#endif /* SENSOR_H_ */
