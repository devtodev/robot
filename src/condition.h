/*
 * condition.h
 *
 *  Created on: Jan 3, 2020
 *      Author: Carlos Miguens
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include "action.h"
#include "sensor.h"

typedef enum {
	EQUALS, BIGGEST, SMALLER
} ConditionType;

typedef struct _Condition {
	SensorType sensor;
	ConditionType type;
	double value;
	ActionType action;
	struct _Condition *next;
} Conditions;

void conditionAdd(SensorType sensor, ConditionType type, double value, ActionType action);
void conditionRemove();


#endif /* CONDITION_H_ */
