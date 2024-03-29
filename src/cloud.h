/*
 * cloud.h
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#ifndef CLOUD_H_
#define CLOUD_H_

#include <netinet/in.h>

#define CLOUD_REFRESH_HZ 10
#define CLOUD_BUFFER_SIZE 1024

typedef enum {OFFLINE, ONLINE} Status;

typedef struct
{
	int socketID;
	int flags;
	struct sockaddr_in socketDestination;
	char buffer[CLOUD_BUFFER_SIZE];
	Status status;
} Connection;

int cloudTelemetryPost();
void* __mqtt_manager(__attribute__ ((unused)) void* ptr);

#endif /* CLOUD_H_ */
