/*
 * telemetry.c
 *
 *  Created on: Aug 17, 2019
 *      Author: Carlos Miguens
 */

#include <stdio.h>
#include <rc/adc.h>
#include <rc/mpu.h>
#include <rc/start_stop.h>

#define I2C_BUS 2
#define VOLTAGE_DISCONNECT	1 // Threshold for detecting disconnected battery

Power power;
Sensors sensors;

uint8_t sensors_init()
{
	rc_mpu_config_t conf = rc_mpu_default_config();
	conf.i2c_bus = I2C_BUS;
	conf.show_warnings = 0; // zero to enable
	sensors.g_mode = G_MODE_DEG;
	sensors.a_mode = A_MODE_MS2;
	if(rc_mpu_initialize(&sensors.data, conf)){
		fprintf(stderr,"rc_mpu_initialize_failed\n");
		return -1;
	}
	return 0;
}

uint8_t telemetry_init()
{
	// ADC init
    if ((-1 == rc_enable_signal_handler()) || (rc_adc_init())){
            fprintf(stderr,"ERROR: failed to run rc_init_adc()\n");
            return -1;
    }
    // init I2C
    rc_mpu_config_t config = rc_mpu_default_config();
	config.i2c_bus = I2C_BUS;
    // calibrate
	if(0>rc_mpu_calibrate_gyro_routine(config)){
		printf("Failed to complete gyro calibration\n");
		return -1;
	}
	if(0>rc_mpu_calibrate_accel_routine(config)){
		printf("Failed to complete accelerometer calibration\n");
		return -1;
	}
	if (0>sensors_init())
	{
		prinf("Failed to initializing sensors");
	}
    return 0;
}

uint8_t refreshPowerStatus()
{
	// read in the voltage of the 2S pack and DC jack
	power.pack_voltage = rc_adc_batt();
	power.jack_voltage = rc_adc_dc_jack();

	// sanity check the SDC didn't return an error
	if(power.pack_voltage<0.0 || power.jack_voltage<0.0){
		return -1;
	}

	// check if a pack is on the 2S balance connector
	if(power.pack_voltage<VOLTAGE_DISCONNECT){
		power.pack_voltage = 0;
	}

	if(power.jack_voltage<VOLTAGE_DISCONNECT){
		power.jack_voltage = 0;
	}

	return 0;
}

uint8_t refreshSensorsStatus()
{
	// read sensor data
	if(0>rc_mpu_read_accel(&sensors.data)){
		printf("read accel data failed\n");
		return -1;
	}
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

uint8_t telemetry_refresh()
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

uint8_t telemetry_shutdown()
{
	rc_adc_cleanup();
	rc_mpu_power_off();
	return 0;
}
