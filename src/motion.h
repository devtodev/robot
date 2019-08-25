/*
 * motion.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Carlos Miguens
 */

#ifndef MOTION_H_
#define MOTION_H_

typedef enum {FORWARD, LEFT, RIGHT, STOP, HIGHTSPEED, LOWSPEED} Action;
typedef enum {HIGH, LOW} Speed;

typedef struct {
	int channel;
	double duty;
} Motor;

int motionDo(Action action);
void motionShutdown();

#endif /* MOTION_H_ */
