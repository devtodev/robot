/*
 * telemetry.c
 *
 *  Created on: Aug 17, 2019
 *      Author: Carlos Miguens
 */

#include <stdio.h>
#include <signal.h>
#include <rc/math.h>
#include <rc/time.h>
#include <rc/adc.h>
#include <rc/start_stop.h>
#include "telemetry.h"
#include "json.h"

# define M_PI		3.14159265358979323846	/* pi */

#define I2C_BUS 2
#define VOLTAGE_DISCONNECT	1 // Threshold for detecting disconnected battery
#define SAMPLE_RATE_HZ		100

Power power;
Sensors sensors;

#define SAMPLE_RATE	50
#define TIME_CONSTANT	2.0
/*
rc_filter_t low_pass	= RC_FILTER_INITIALIZER;
rc_filter_t high_pass	= RC_FILTER_INITIALIZER;
rc_filter_t integrator	= RC_FILTER_INITIALIZER;
rc_filter_t lp_butter	= RC_FILTER_INITIALIZER;
rc_filter_t hp_butter	= RC_FILTER_INITIALIZER; */
const double dt = 1.0/SAMPLE_RATE;
double zGyroFiltered;

uint8_t vars_init()
{
	// gyro
	rc_filter_t zGyroFilter = RC_FILTER_INITIALIZER;

	// init filters
	/*	rc_filter_first_order_highpass(&low_pass, dt, TIME_CONSTANT);
	rc_filter_butterworth_highpass(&hp_butter, 2, dt, 2.0*M_PI/TIME_CONSTANT);
	rc_filter_first_order_highpass(&high_pass, dt, TIME_CONSTANT);
	rc_filter_integrator(&integrator, dt);
	rc_filter_butterworth_lowpass(&lp_butter, 2, dt, 2.0*M_PI/TIME_CONSTANT);
	rc_filter_butterworth_highpass(&hp_butter, 2, dt, 2.0*M_PI/TIME_CONSTANT);
*/

}

uint8_t sensors_init()
{
	rc_mpu_config_t conf = rc_mpu_default_config();
	conf.i2c_bus = I2C_BUS;
	conf.show_warnings = 0; // zero to enable
	conf.dmp_sample_rate = SAMPLE_RATE_HZ;
	conf.orient = ORIENTATION_Z_UP;

	// if gyro isn't calibrated, run the calibration routine
	if(!rc_mpu_is_gyro_calibrated()){
		printf("Gyro not calibrated, automatically starting calibration routine\n");
		printf("Let your MiP sit still on a firm surface\n");
		rc_mpu_calibrate_gyro_routine(conf);
	}

	sensors.g_mode = G_MODE_DEG;
	sensors.a_mode = A_MODE_MS2;
	if(rc_mpu_initialize(&sensors.data, conf)){
		fprintf(stderr,"rc_mpu_initialize_failed\n");
		return -1;
	}

	return 0;
}

uint8_t telemetryInit()
{
	// ADC init
    if ((-1 == rc_enable_signal_handler()) ||
    	(-1 == rc_adc_init()))
    {
            fprintf(stderr,"ERROR: failed to run rc_init_adc()\n");
            return -1;
    }
    if (0>sensors_init())
	{
		printf("Failed to initializing sensors");
		return -1;
	}
    return 0;
}

double zf = 0 , z = 0;
void telemetryReport(char *buffer)
{
	jsonBegin(buffer);
	jsonKeyDouble("temperature\0", sensors.data.temp, buffer);
	jsonObjBegin("accel\0", buffer);
	jsonKeyFloat("x\0", sensors.data.accel[0], buffer);
	jsonKeyFloat("y\0", sensors.data.accel[1], buffer);
	jsonKeyFloat("z\0", sensors.data.accel[2], buffer);
	jsonObjEnd(buffer);
	jsonObjBegin("gyro\0", buffer);
	jsonKeyInt("x\0", (int) sensors.data.gyro[0], buffer);
	jsonKeyInt("y\0", (int) zf, buffer);
	jsonKeyInt("z\0", (int) z, buffer);
	jsonObjEnd(buffer);
	jsonKeyDouble("battery\0", power.batt_voltage, buffer);
	jsonKeyDouble("jack\0", power.jack_voltage, buffer);
	jsonEnd(buffer);
}

uint8_t refreshPowerStatus()
{
	// read in the voltage of the 2S pack and DC jack
	power.batt_voltage = rc_adc_batt();
	power.jack_voltage = rc_adc_dc_jack();

	// sanity check the SDC didn't return an error
	if(power.batt_voltage<0.0 || power.jack_voltage<0.0)
	{
		return -1;
	}

	// check if a pack is on the 2S balance connector
	if(power.batt_voltage<VOLTAGE_DISCONNECT){
		power.batt_voltage = 0;
	}

	if(power.jack_voltage<VOLTAGE_DISCONNECT){
		power.jack_voltage = 0;
	}

	return 0;
}

void signalFiltering(double data)
{
	// march all filters one step forward with u as the common input.
	// new outputs saved as lp,hp,and i. complement is lp+hp
//	zGyroFiltered = rc_filter_march(&hp_butter, data);
/* p = rc_filter_march(&high_pass, data);
	i  = rc_filter_march(&integrator, data);
	lpb = rc_filter_march(&lp_butter, data);
	hpb = rc_filter_march(&hp_butter, data);

	printf("\r");
	printf("%8.3f  |", data);
	printf("%8.3f  |", lp);
	printf("%8.3f  |", hp);
	printf("%8.3f  |", lp+hp);
	printf("%8.3f  |", i);
	printf("%8.3f  |", lpb);
	printf("%8.3f  |", hpb); */
}

uint8_t refreshSensorsStatus()
{
	// read sensor data
	if(0>rc_mpu_read_accel(&sensors.data)){
		printf("read accel data failed\n");
		return -1;
	}
	signalFiltering(sensors.data.gyro[Z]);
	if(0>rc_mpu_read_gyro(&sensors.data)){
		printf("read gyro data failed\n");
		return -1;
	}
	if(0>rc_mpu_read_temp(&sensors.data)) {
		printf("read imu thermometer failed\n");
		return -1;
	}
	return 0;
}

uint8_t telemetryRefresh()
{
	if(0>refreshPowerStatus()){
		printf("ERROR: can't read voltages\n");
		return -1;
	}
	if(0>refreshSensorsStatus()){
		printf("ERROR: can't read sensors\n");
		return -1;
	}
	return 0;
}

void telemetryPrintVars()
{
	printf("\n|===========================================|");
	printf("\n| temp:   %4.1f                              |", sensors.data.temp);
	printf("\n| gyro: %6.1f %6.1f %6.1f                | ", sensors.data.gyro[0], sensors.data.gyro[1], sensors.data.gyro[2], zGyroFiltered);
	printf("\n| acce:  %6.2f %6.2f %6.2f               | ", sensors.data.accel[0], sensors.data.accel[1], sensors.data.accel[2]);
	printf("\n|-------------------------------------------|");
	printf("\n| Pack:   %0.2lfV DC Jack: %0.2lfV              | ", power.batt_voltage, power.jack_voltage);
	printf("\n|===========================================|");
	fflush(stdout);
}

Power getPower()
{
	return power;
}

double getGyroZ()
{
	return zGyroFiltered;
}

void telemetryShutdown()
{
	rc_adc_cleanup();
	rc_mpu_power_off();
}
