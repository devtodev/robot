/*
 * brain.c
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#include "brain.h"
#include "dashboard.h"
#include "action.h"
#include "stdio.h"

Dashboard dashboard;

void setDashboard()
{
	dashboard.rules  = NULL;
	dashboard.score = 0;
}

void setDefaultBrian()
{
}

void brainRefresh()
{
	ActionType action = getAction();
	if (action != NONE)
	{
		actionExecute(action);
	}
}

void brainShutdown()
{
}
