/*
 * brain.h
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#ifndef BRAIN_H_
#define BRAIN_H_

#include "sensor.h"
#include "action.h"

typedef enum {
	EQUALS, BIGGEST, SMALLER
} ConditionType;

typedef struct _Condition {
	ConditionType type;
	Sensor sensor;
	Action action;
	int score;
	struct _Condition *next;
} Condition;


Sensor gyro;

#endif /* BRAIN_H_ */
