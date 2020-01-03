/*
 * camera.c
 *
 *  Created on: Jan 1, 2020
 *      Author: karl
 */

#include <rc/servo.h>
#include <rc/adc.h>
#include <stdio.h>
#include "camera.h"

double angle;

int cameraInit()
{
	// read adc to make sure battery is connected
	if(rc_adc_init()){
		fprintf(stderr,"ERROR: failed to run rc_adc_init()\n");
		return -1;
	}
	rc_adc_cleanup();

	// initialize PRU
	if(rc_servo_init()) return -1;

	// turn on power
	printf("Turning On 6V Servo Power Rail\n");
	rc_servo_power_rail_en(1);
	angle = 0;
	return 0;
}

void cameraSetPosition(ActionType cameraAngle)
{
	switch (cameraAngle)
	{
		case CAMERA_LEFT:
			angle = -1.5;
			break;
		case CAMERA_HALF_LEFT:
			angle = -0.7;
			break;
		case CAMERA_CENTER:
			angle = 0;
			break;
		case CAMERA_HALF_RIGHT:
			angle = 0.7;
			break;
		case CAMERA_RIGHT:
			angle = 1.5;
			break;
		default:
			angle = 0;
			break;
	}
}

int cameraRefresh()
{
//	printf("angle %f \n", angle);
	if(rc_servo_send_pulse_normalized(CAMERA_SERVO,angle)==-1)
		return -1;
	return 0;
}

