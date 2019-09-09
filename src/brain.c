/*
 * brain.c
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#include "brain.h"

Sensors sensors;
Actions actions;
Brain brain;

Brain* getDefaultBrian()
{
	brain.sensors = &sensors;
	initSensor(brain.sensors, GYRO, DEG, HIGH_PASS, gyroInit, gyroRefresh, gyroClose);
	addSensor(brain.sensors, ACC, M2S, HIGH_PASS, accInit, accRefresh, accClose);
	addSensor(brain.sensors, US, METER, LOW_PASS, usInit, usRefresh, usClose);
	addSensor(brain.sensors, BATTERY, VOLT, LOW_PASS, batteryInit, batteryRefresh, batteryClose);
	addSensor(brain.sensors, JACK, VOLT, LOW_PASS, jackInit, jackRefresh, jackClose);
	return &brain;
}

Brain* brainInit(Sensors *sensors, Actions *actions, Dashboard *dashboad)
{
	brain.sensors = sensors;
	brain.actions = actions;
	brain.dashboad = dashboad;
	return &brain;
}

Sensors *refreshSensors()
{
	Sensors *cursor = brain.sensors;
	while (cursor != NULL) {
		cursor->refreshSensor();
		cursor = cursor->next();
	}
	return brain.sensors;
}
