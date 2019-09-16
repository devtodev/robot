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

Sensors sensors;
Actions actions;
Dashboard dashboard;

void setDashboard()
{
	dashboard.point = NULL;
	dashboard.score = 0;
}

void setDefaultSensors()
{
	initSetSensor(&sensors, GYRO, DEG, HIGH_PASS, gyroInit, gyroRefresh, gyroClose);
	addSensor(&sensors, ACC, M2S, HIGH_PASS, accInit, accRefresh, accClose);
	addSensor(&sensors, US, METER, LOW_PASS, usInit, usRefresh, usClose);
	addSensor(&sensors, TEMP, C, LOW_PASS, tempInit, tempRefresh, tempClose);
	addSensor(&sensors, BATTERY, VOLT, LOW_PASS, batteryInit, batteryRefresh, batteryClose);
	addSensor(&sensors, JACK, VOLT, LOW_PASS, jackInit, jackRefresh, jackClose);
}

void setDefaultBrian()
{
	setDefaultSensors();
}

void brainRefresh()
{
	Sensors *cursor = &sensors;
	while (cursor != NULL) {
		cursor->refreshSensor();
		printf("%4.1f \n", cursor->value);
		cursor = cursor->next;
	}
}

