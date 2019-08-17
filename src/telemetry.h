/*
 * telemetry.h
 *
 *  Created on: Aug 17, 2019
 *      Author: Carlos Miguens
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <inttypes.h>

// possible modes, user selected with command line arguments
typedef enum g_mode_t{
	G_MODE_RAD,
	G_MODE_DEG,
	G_MODE_RAW
} g_mode_t;

typedef enum a_mode_t{
	A_MODE_MS2,
	A_MODE_G,
	A_MODE_RAW
} a_mode_t;

typedef struct {
	double pack_voltage;	// 2S pack voltage on JST XH 2S balance connector
	double jack_voltage;	// could be dc power supply or another battery
} Power;

typedef struct {
	rc_mpu_data_t data; 	//struct to hold new data
	g_mode_t g_mode; 		// gyro default to degree mode.
	a_mode_t a_mode; 		// accel default to m/s^2
} Sensors;

uint8_t telemetry_refresh();
uint8_t telemetry_init();
void telemetry_shutdown();

#endif /* TELEMETRY_H_ */
