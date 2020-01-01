/*
 * action.h
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#ifndef ACTION_H_
#define ACTION_H_

#include "time.h"
#include "utils.h"

typedef enum {
	FORWARD, BACKWARD, RIGHT, LEFT, STOP, OPENCV, CLOSECV, HIGHTSPEED, LOWSPEED, SERVO_RIGHT, SERVO_LEFT, SERVO_STOP, RC_LED_GREEN_ON, RC_LED_GREEN_OFF, RC_LED_RED_ON, RC_LED_RED_OFF
} ActionType;

typedef struct _Action{
	ActionType type;
	time_t start;
	time_t end;
	int score;
	struct _Action *next;
} Actions;


#endif /* ACTION_H_ */
