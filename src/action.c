/*
 * action.c
 *
 *  Created on: Sep 8, 2019
 *      Author: karl
 */

#include "stdlib.h"
#include "action.h"
#include "motion.h"
#include "sensor.h"
#include "camera.h"


int nActions = 0;
Action *rootAction;

int addAction(ActionType action)
{
	Action *cursor = rootAction;
	if (nActions == 0)
	{
		rootAction = malloc(sizeof(ActionType));
		cursor = rootAction;
	} else {
		for (int i = 0; (i < nActions) && (cursor->next != NULL); i++)
		{
			cursor = cursor->next;
		}
		cursor->next = malloc(sizeof(ActionType));
		cursor = cursor->next;
	}
	cursor->type = action;
	cursor->next = NULL;
	nActions++;
	return nActions;
}

ActionType getAction()
{
	if (nActions < 1)
		return NONE;
	Action *cursor = rootAction;
	ActionType action = cursor->type;
	rootAction = cursor->next;
	free(cursor);
	nActions--;
	return action;
}

int actionExecute(ActionType action)
{
	if ((action == MOVE_FORWARD) || (action == MOVE_STOP) ||
		(action == MOVE_RIGHT) || (action == MOVE_LEFT) ||
		(action == HIGHTSPEED) || (action == LOWSPEED))
	{
		motionDo(action);
	}

	if ((action == CAMERA_LEFT) || (action == CAMERA_HALF_LEFT) || (action == CAMERA_CENTER)  ||
		(action == CAMERA_HALF_RIGHT) || (action == CAMERA_RIGHT))
	{
		cameraSetPosition(action);
	}

	return 0;
}
