/*
 * sensor.h
 *
 *  Created on: Sep 8, 2019
 *      Author: karl
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "time.h"

typedef enum {
	ACC, GYRO, US
} SensorType;

typedef enum {
	RAW, M2S, DEG, RAD
} UnitType;

typedef enum {
	LOW_PASS, HIGH_PASS, INTEGRATOR, LP_BUTTER, HP_BUTTER
} FilterType;

typedef struct {
	SensorType type;
	double value;
	UnitType unit;
	FilterType filter;
	time_t start;
	time_t end;
	int score;
	Sensor *next;
} Sensor;

#endif /* SENSOR_H_ */
