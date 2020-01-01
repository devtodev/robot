/*
 * camera.h
 *
 *  Created on: Jan 1, 2020
 *      Author: karl
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#define CAMERA_SERVO 8
#define CAMERA_HZ 50

typedef enum {
	CAMERA_LEFT, CAMERA_HALF_LEFT, CAMERA_CENTER, CAMERA_HALF_RIGHT, CAMERA_RIGHT
} CameraAngle;

int cameraInit();
void cameraSetPosition(CameraAngle angle);
int cameraRefresh();

#endif /* SRC_CAMERA_H_ */
