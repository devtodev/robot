/*
 * sensor.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Carlos Miguens
 */

#include "sensor.h"
#include "stdlib.h"

Sensors *addSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	Sensors *sensorCursor =  malloc(sizeof(Sensors));
	initSensor(sensorCursor, sensorType, unit, filter, initSensor, refreshSensor, closeSensor);
	while (sensors->next != NULL)
	{
		sensors = sensors->next;
	}
	sensors->next = sensorCursor;
	return sensors->next;
}

Sensors *initSensor(Sensors *sensors, SensorType sensorType, UnitType unit, FilterType filter,
			   void (*initSensor)(), void (*refreshSensor)(), void (*closeSensor)())
{
	sensors->type = sensorType;
	sensors->filter = filter;
	sensors->score = 0;
	sensors->value = 0;
	sensors->unit = unit;
	sensors->start = NULL;
	sensors->end = NULL;
	sensors->refreshSensor = refreshSensor;
	sensors->initSensor = initSensor;
	sensors->closeSensor = closeSensor;
	sensors->next = NULL;
	return sensors;
}

void gyroInit() {}
void gyroRefresh() {}
void gyroClose() {}

void accInit() {}
void accRefresh() {}
void accClose() {}

void usInit() {}
void usRefresh() {}
void usClose() {}

void batteryInit() {}
void batteryRefresh() {}
void batteryClose() {}

void jackInit() {}
void jackRefresh() {}
void jackClose() {}
