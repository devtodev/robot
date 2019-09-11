/*
 * brain.h
 *
 *  Created on: Sep 8, 2019
 *      Author: Carlos Miguens
 */

#ifndef BRAIN_H_
#define BRAIN_H_

#include "sensor.h"
#include "action.h"
#include "dashboard.h"

typedef struct {
	Sensors *sensors;
	Actions *actions;
	Dashboard *dashboard;
	// cloud
	// files
	time_t now;
} Brain;


void brainRefresh(Brain* brain);
Brain* getDefaultBrian();

#endif /* BRAIN_H_ */
