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

Actions actions;
Dashboard dashboard;

void setDashboard()
{
	dashboard.point = NULL;
	dashboard.score = 0;
}

void setDefaultBrian()
{
}

void brainRefresh()
{
}

void brainShutdown()
{
}
