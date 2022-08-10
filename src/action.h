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
	OPENCV, CLOSECV, HIGHTSPEED, LOWSPEED, RC_LED_GREEN_ON, RC_LED_GREEN_OFF, RC_LED_RED_ON, RC_LED_RED_OFF,
	MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_STOP,
	RIGHT_180, LEFT_180,
	CAMERA_LEFT, CAMERA_HALF_LEFT, CAMERA_CENTER, CAMERA_HALF_RIGHT, CAMERA_RIGHT,
	NONE
} ActionType;

typedef struct _Action{
	ActionType type;
	struct _Action *next;
} Action;

ActionType getAction();
int actionExecute(ActionType action);
int addAction(ActionType action);


#endif /* ACTION_H_ */
