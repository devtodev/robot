/*
 * motion.c
 *
 *  Created on: Aug 23, 2019
 *      Author: Carlos Miguens
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motion.h"
#include <rc/motor.h>

#define LEFT_FORWARD        1  // to solve inverted cables
#define RIGHT_FORWARD       1

#define LEFT_CHANNEL_MOTOR  1
#define RIGHT_CHANNEL_MOTOR 3

Motor motorLeft;
Motor motorRight;
Speed speed;
double motorFreqHZ;

void updateMotor(const Motor motor)
{
	rc_motor_set(motor.channel,motor.duty);
}

void motionInit()
{
	motorLeft.channel = LEFT_CHANNEL_MOTOR;
	motorLeft.duty = 1;
	motorRight.channel = RIGHT_CHANNEL_MOTOR;
	motorRight.duty = 1;
	// init hardware
	motorFreqHZ = RC_MOTOR_DEFAULT_PWM_FREQ;
	//rc_motor_init_freq(motorFreqHZ);
	// to test
	 	rc_motor_init();
	//  rc_motor_free_spin(int ch);
}

void moveForward()
{
	motorLeft.duty = LEFT_FORWARD;
	motorRight.duty = RIGHT_FORWARD;
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void turnLeft()
{
	motorLeft.duty = LEFT_FORWARD  * -1;
	motorRight.duty = RIGHT_FORWARD;
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void turnRight()
{
	motorLeft.duty = LEFT_FORWARD;
	motorRight.duty = RIGHT_FORWARD * -1;
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void highSpeed()
{
	speed = HIGH;
}

void lowSpeed()
{
	speed = LOW;
}

void stop()
{
	rc_motor_brake(motorLeft.channel);
	rc_motor_brake(motorRight.channel);
}

int motionDo(Action action)
{
	switch (action)
	{
		case FORWARD:
			moveForward();
			break;
		case LEFT:
			turnLeft();
			break;
		case RIGHT:
			turnRight();
			break;
		case STOP:
			stop();
			break;
		case HIGHTSPEED:
			highSpeed();
			break;
		case LOWSPEED:
			lowSpeed();
			break;
		default:
			return -1;
			break;
	}
	return 0;
}

void motionShutdown()
{
	rc_motor_cleanup();
}


