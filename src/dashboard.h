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
	Sensors *sensors;
	Actions *action; // action to do when the condition was targeted
	int score;
	struct _Condition *next;
} Conditions;

typedef struct _Point {
	int score;
	Conditions *condition;
	Actions *learning; // action path to match condition
	time_t timestamp;
	int done;
	struct _Point *next;
} Rule;

typedef struct {
	int score;
	Rule *rules;
} Dashboard;

void initRules(Rule *rule, Conditions *condition, int score, Actions *learning);
void addRule(Rule *rule, Conditions *condition, int score, Actions *learning);
#endif /* DASHBOARD_H_ */
