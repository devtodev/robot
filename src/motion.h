/*
 * motion.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Carlos Miguens
 */

#ifndef MOTION_H_
#define MOTION_H_

#include "action.h"

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
int motionDo(ActionType action);
void motionControl(double gyro[3]);
void motionShutdown();

#endif /* MOTION_H_ */
