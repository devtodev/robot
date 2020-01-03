/*
 * dashboard.h
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include "time.h"
#include "action.h"
#include "sensor.h"

typedef enum {
	EQUALS, BIGGEST, SMALLER
} ConditionType;

typedef struct _Condition {
	ConditionType type;
	SensorType sensor;
	Action *action; // action to do when the condition was targeted
	int score;
	struct _Condition *next;
} Conditions;

typedef struct _Point {
	int score;
	Conditions *condition;
	Action *learning; // action path to match condition
	time_t timestamp;
	int done;
	struct _Point *next;
} Rule;

typedef struct {
	int score;
	Rule *rules;
} Dashboard;

void setRule(Rule *rules, Conditions *condition, int score, Action *learning);
void addRule(Rule *rules, Conditions *condition, int score, Action *learning);
#endif /* DASHBOARD_H_ */
