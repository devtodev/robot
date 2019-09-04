/*
 * motion.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Carlos Miguens
 */

#ifndef MOTION_H_
#define MOTION_H_

#include "telemetry.h"	// to use in PID algorithms, TODO: decouple

typedef enum {FORWARD, LEFT, RIGHT, STOP, HIGHTSPEED, LOWSPEED} Action;

typedef struct {
	int channel;
	double duty;
} Motor;

typedef struct {
	double error;
	double lastError;
	double integral;
	double derivative;
	double kp;
	double ki;
	double kd;
	double correction;
} PID;

void motionInit();
int motionDo(Action action);
void motionControl(Sensors sensors);
void motionShutdown();

#endif /* MOTION_H_ */
