/*
 * cloud.h
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#ifndef CLOUD_H_
#define CLOUD_H_

#include <netinet/in.h>

#define CLOUD_BUFFER_SIZE 1024

typedef struct
{
	int socketID;
	int flags;
	struct sockaddr_in socketDestination;
	char buffer[CLOUD_BUFFER_SIZE];
} Connection;

int cloudConnect();
int cloudReadData();
int cloudTelemetryPost(Sensors sensors, Power power);
void cloudShutDown();

#endif /* CLOUD_H_ */
