/*
 * main.c
 *
 *  Created on: Aug 17, 2019
 *      Author: karl
 */

#include <stdio.h>
#include "telemetry.h"
#include "cloud.h"
#include "motion.h"
#include "rc/start_stop.h"
#include "rc/led.h"
#include "rc/time.h"

#define TIMEDELAY 			100000 // microseconds
#define TIMETOTELEMETRY		20	   // Multiplier of TIMEDELAY
typedef enum {
	DEBUG,
	PRODUCTION,
} ProgramMode;

int programMode = DEBUG;

int programInit()
{
	// make sure another instance isn't running
	// if return value is -3 then a background process is running with
	// higher privaledges and we couldn't kill it, in which case we should
	// not continue or there may be hardware conflicts. If it returned -4
	// then there was an invalid argument that needs to be fixed.
	if(rc_kill_existing_process(2.0)<-2) return -1;

	// start signal handler so we can exit cleanly
	if(rc_enable_signal_handler()<0){
		printf("ERROR: failed to complete rc_enable_signal_handler\n");
		return -1;
	}
	// make PID file to indicate your project is running
	// due to the check made on the call to rc_kill_existing_process() above
	// we can be fairly confident there is no PID file already and we can
	// make our own safely.
	rc_make_pid_file();
	// prepare to run
	rc_set_state(RUNNING);
	return 0;
}

int main(void)
{
	int toggle = 0;			// led status
	int timeToTelemetry = 0;
	motionInit();
    cloudInit();
	if ((0>programInit()) ||
		(0>telemetryInit()))
	{
		return -1;
	}
	// Run the main loop until state is EXITING which is set by hitting ctrl-c
	// or holding down the pause button for more than the quit timeout period
	while(rc_get_state()!=EXITING){
		// if the state is RUNNING (instead of PAUSED) then blink!
		if(rc_get_state()==RUNNING){
			if(toggle){
				rc_led_set(RC_LED_GREEN,0);
				rc_led_set(RC_LED_RED,1);
				toggle = 0;
			}
			else{
				rc_led_set(RC_LED_GREEN,1);
				rc_led_set(RC_LED_RED,0);
				toggle=1;
			}
		}
		// TODO: REFACTOR THIS!
		if (getConnectionStatus() == OFFLINE)
		{
			cloudConnect();
		} else {
			telemetryRefresh();

			if (0 < cloudReadData())
			{
				// acknowledgement
			}

			if (TIMETOTELEMETRY < timeToTelemetry)
			{
				timeToTelemetry = 0;
				// time to flush
				rc_usleep(1000);
				cloudTelemetryPost(getSensors(), getPower());
			} else {
				timeToTelemetry++;
			}
		}
		if (programMode == DEBUG)
		{
			//telemetryPrintVars();
		}
		// sleep the right delay based on current mode.
		rc_usleep(TIMEDELAY);
	}
	// program shutdown
	rc_led_set(RC_LED_GREEN, 0);
	rc_led_set(RC_LED_RED, 0);
	telemetryShutdown();
	rc_remove_pid_file();
	cloudShutDown();
	motionShutdown();
	return 0;
}
