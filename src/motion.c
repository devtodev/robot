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
#include "action.h"

#define LEFT_FORWARD        1  // to solve inverted cables
#define RIGHT_FORWARD       1

#define LEFT_CHANNEL_MOTOR  1
#define RIGHT_CHANNEL_MOTOR 3

Motor motorLeft;
Motor motorRight;
double motorFreqHZ;
ActionType currentAction;
PID motorsPID;

void updateMotor(const Motor motor)
{
	rc_motor_set(motor.channel,motor.duty);
}

void motionInit()
{
	currentAction = STOP;
	// init motor PID
	motorsPID.kp = 0;
	motorsPID.ki = 0;
	motorsPID.kd = 1;
	motorRight.duty = 1;
	motorLeft.duty = 1;
	// init motors
	motorLeft.channel = LEFT_CHANNEL_MOTOR;
	motorLeft.duty = 1;
	motorRight.channel = RIGHT_CHANNEL_MOTOR;
	motorRight.duty = 1;
	// init encoders
	/*
	if(rc_encoder_eqep_init()==-1){
		fprintf(stderr,"ERROR: failed to initialize eqep encoders\n");
		return;
	}*/
	// init hardware
	motorFreqHZ = RC_MOTOR_DEFAULT_PWM_FREQ;
	rc_motor_init_freq(motorFreqHZ);
	rc_motor_standby(1);
	// to test
	//rc_motor_init();
	//  rc_motor_free_spin(int ch);
}

void moveForward()
{
	motorLeft.duty = LEFT_FORWARD * motorLeft.duty;
	motorRight.duty = RIGHT_FORWARD * motorRight.duty;
	rc_motor_standby(0);
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void turnLeft()
{
	motorLeft.duty = LEFT_FORWARD  * -1;//  * motorLeft.duty;
	motorRight.duty = RIGHT_FORWARD;//  * motorRight.duty;
	rc_motor_standby(0);
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void turnRight()
{
	motorLeft.duty = LEFT_FORWARD;// * motorLeft.duty;
	motorRight.duty = RIGHT_FORWARD * -1;// * motorRight.duty;
	rc_motor_standby(0);
	updateMotor(motorLeft);
	updateMotor(motorRight);
}

void stop()
{
	rc_motor_brake(motorLeft.channel);
	rc_motor_brake(motorRight.channel);
	rc_motor_standby(1);
}

int motionDo(ActionType action)
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
			motorLeft.duty = 1;
			motorRight.duty = 1;
			stop();
			break;
		case HIGHTSPEED:
			motorLeft.duty = 1;
			motorRight.duty = 1;
			break;
		case LOWSPEED:
			motorLeft.duty = 0.5;
			motorRight.duty = 0.5;
			break;
		case SERVO_LEFT:
			break;
		case SERVO_RIGHT:
			break;
		case SERVO_STOP:
			break;
		default:
			return -1;
			break;
	}
	currentAction = action;
	return 0;
}

void speedCorrection(int factor)
{
	motorLeft.duty =  motorLeft.duty + factor / 10;
	motorRight.duty = motorRight.duty - factor / 10;
	motorLeft.duty = (motorLeft.duty > 1)?1:motorLeft.duty;
	motorLeft.duty = (motorLeft.duty < -1)?-1:motorLeft.duty;
	motorRight.duty = (motorRight.duty > 1)?1:motorRight.duty;
	motorRight.duty = (motorRight.duty < -1)?-1:motorRight.duty;

	//updateMotor(motorLeft);
	//updateMotor(motorRight);
}

void motionControl(double gyro[3])
{
	if (currentAction == FORWARD)
	{
		/* motorsPID.lastError = motorsPID.error;
		motorsPID.error =  0 - sensors.data.gyro[Z];
		motorsPID.integral = motorsPID.integral + motorsPID.error;
		motorsPID.derivative = motorsPID.error - motorsPID.lastError;
		motorsPID.correction = motorsPID.kp * motorsPID.error +
							   motorsPID.ki * motorsPID.integral +
							   motorsPID.kd * motorsPID.derivative; */
		speedCorrection(motorsPID.correction);
	}
}

void motionShutdown()
{
	rc_motor_cleanup();
}


