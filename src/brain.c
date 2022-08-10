/*
 * brain.c
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#include "brain.h"
#include "action.h"
#include "stdio.h"
#include "condition.h"
#include "camera.h"


void brainRefresh()
{
	ActionType action = getAction();
	cameraRefresh();
	//sensorsRefresh();
	if (action != NONE)
	{
		actionExecute(action);
	}
}
