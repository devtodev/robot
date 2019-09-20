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
#include "rc/time.h"
#include "rc/pthread.h"
#include "brain.h"

#define TIMEDELAY 			10000 // microseconds
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
	pthread_t sensors_thread = 0;

    programInit();

	// start sensor thread
	if(rc_pthread_create(&sensors_thread, __sensor_manager, (void*) NULL, SCHED_OTHER, 0))
	{
		fprintf(stderr, "failed to start sensors thread\n");
		return -1;
	}

	while(rc_get_state()!=EXITING){
		brainRefresh();
		// sleep the right delay based on current mode.
		rc_usleep(TIMEDELAY);
	}

	if (sensors_thread) rc_pthread_timed_join(sensors_thread, NULL, 1.5);

	rc_remove_pid_file();
	return 0;
}
