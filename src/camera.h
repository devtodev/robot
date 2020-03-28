/*
 * camera.h
 *
 *  Created on: Jan 1, 2020
 *      Author: Carlos Miguens
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#define CAMERA_SERVO 8
#define CAMERA_HZ 50

#include "action.h"

int cameraInit();
void cameraSetPosition(ActionType angle);
int cameraRefresh();

#endif /* SRC_CAMERA_H_ */
