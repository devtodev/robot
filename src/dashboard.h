/*
 * dashboard.h
 *
 *  Created on: Sep 8, 2019
 *      Author: karl
 */

#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include "time.h"

typedef enum {
	EQUALS, BIGGEST, SMALLER
} ConditionType;

typedef struct _Condition {
	ConditionType type;
	Sensors *sensors;
	Actions *action;
	int score;
	struct _Condition *next;
} Conditions;

typedef struct _Point {
	int score;
	Conditions condition;
	Actions *learning; // action path to match condition
	time_t done;
	struct _Point *next;
} Point;

typedef struct {
	int wishLen;
	Point wishes;
} Wishlist;

typedef struct {
	int score;
	Point points;
	Wishlist rules;
} Dashboard;

#endif /* DASHBOARD_H_ */
