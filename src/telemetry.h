/*
 * telemetry.h
 *
 *  Created on: Aug 17, 2019
 *      Author: Carlos Miguens
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <inttypes.h>
#include "rc/mpu.h"


#define X 0
#define Y 1
#define Z 2


typedef struct {
	double batt_voltage;	// 2S pack voltage on JST XH 2S balance connector
	double jack_voltage;	// could be dc power supply or another battery
} Power;

typedef struct {
	rc_mpu_data_t data; 	// struct to hold new data
} Sensor;

uint8_t telemetryRefresh();
uint8_t telemetryInit();
void telemetryReport(char *buffer);
void telemetryPrintVars();
double getGyroZ();
Power getPower();
void telemetryShutdown();

#endif /* TELEMETRY_H_ */
