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
Brain brain;

Brain* getDefaultBrian()
{

	brain.sensors = &sensors;
	initSensor(brain.sensors, GYRO, DEG, HIGH_PASS, gyroInit, gyroRefresh, gyroClose);
	addSensor(brain.sensors, ACC, M2S, HIGH_PASS, accInit, accRefresh, accClose);
	addSensor(brain.sensors, US, METER, LOW_PASS, usInit, usRefresh, usClose);
	addSensor(brain.sensors, BATTERY, VOLT, LOW_PASS, batteryInit, batteryRefresh, batteryClose);
	addSensor(brain.sensors, JACK, VOLT, LOW_PASS, jackInit, jackRefresh, jackClose);
	brain.actions = &actions;
	brain.dashboard = &dashboard;
	return &brain;
}

void refreshSensors(Sensors *sensors)
{
	Sensors *cursor = sensors;
	while (cursor != NULL) {
		cursor->refreshSensor();
		printf("%4.1f \n", cursor->value);
		cursor = cursor->next;
	}
}

void brainRefresh(Brain* brain)
{
	refreshSensors(brain->sensors);
}
