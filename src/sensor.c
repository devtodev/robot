/*
 * sensor.c
 *
 *  Created on: Sep 9, 2019
 *      Author: Carlos Miguens
 */

#include "sensor.h"
#include "rc/mpu.h"
#include "rc/adc.h"
#include "rc/time.h"
#include "rc/start_stop.h"
#include "stdlib.h"
#include "stdio.h"
#include "json.h"

rc_mpu_data_t sensor;
char *buffer;
Power power;

char *getTelemetryReport()
{
	//rc_mpu_read_temp(&rawSensor.data);
	jsonBegin(buffer);
	jsonKeyDouble("temperature\0", sensor.temp, buffer);
	jsonObjBegin("accel\0", buffer);
	jsonKeyFloat("x\0", sensor.accel[0], buffer);
	jsonKeyFloat("y\0", sensor.accel[1], buffer);
	jsonKeyFloat("z\0", sensor.accel[2], buffer);
	jsonObjEnd(buffer);
	jsonObjBegin("gyro\0", buffer);
	jsonKeyDouble("x\0", sensor.compass_heading*RAD_TO_DEG, buffer);
	jsonKeyDouble("y\0", sensor.compass_heading_raw*RAD_TO_DEG, buffer);
	jsonKeyInt("z\0", (int) sensor.gyro[2], buffer);
	jsonObjEnd(buffer);
	jsonKeyDouble("battery\0", power.batt_voltage, buffer);
	jsonKeyDouble("jack\0", power.jack_voltage, buffer);
	jsonEnd(buffer);
	return buffer;
}

void temperatureRefresh() {
	rc_mpu_read_temp(&sensor);
}

void batteryRefresh() {
	// read in the voltage of the 2S pack and DC jack
	power.batt_voltage = rc_adc_batt();

	// check if a pack is on the 2S balance connector
	if(power.batt_voltage<VOLTAGE_DISCONNECT){
		power.batt_voltage = 0;
	}
}

void batteryClose() {
	rc_adc_cleanup();
	rc_mpu_power_off();
}

void jackRefresh() {
	power.jack_voltage = rc_adc_dc_jack();

	if(power.jack_voltage<VOLTAGE_DISCONNECT){
		power.jack_voltage = 0;
	}

}

void jackClose() {
	rc_adc_cleanup();
	rc_mpu_power_off();
}

void sensorsInit()
{
	buffer = malloc(4096);
	rc_mpu_config_t conf = rc_mpu_default_config();
	conf.i2c_bus = I2C_BUS;
	conf.gpio_interrupt_pin_chip = GPIO_INT_PIN_CHIP;
	conf.gpio_interrupt_pin = GPIO_INT_PIN_PIN;
	conf.show_warnings = 0; // zero to enable
	conf.enable_magnetometer = 1;
	conf.dmp_fetch_accel_gyro=1;

	// now set up the imu for dmp interrupt operation
	if(rc_mpu_initialize_dmp(&sensor, conf)){
		printf("rc_mpu_initialize_failed\n");
		return;
	}

}

void sensorsRefresh()
{
	temperatureRefresh();
	batteryRefresh();
	jackRefresh();
}

void sensorsShutdown()
{

}
