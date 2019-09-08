/*
 * action.h
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#ifndef ACTION_H_
#define ACTION_H_

#include "time.h"

typedef enum {
	FORWARD, BACKWARD, RIGHT, LEFT, STOP, OPENCV, CLOSECV, HIGHTSPEED, LOWSPEED
} ActionType;

typedef struct _Action{
	ActionType type;
	time_t start;
	time_t end;
	int score;
	struct _Action *next;
} Action;

#endif /* ACTION_H_ */
