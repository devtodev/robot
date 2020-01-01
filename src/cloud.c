/*
 * cloud.c
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#include "cloud.h"
#include "motion.h" // Decouple this
#include "sensor.h" // Decouple this
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "rc/time.h"
#include "rc/start_stop.h"
#include <errno.h>
#include "camera.h"

#define MAXRCVLEN 500
#define MSG_WELCOME "Welcome to the jaguar house..."

Connection connection;

int errorCheck(int n, char * err)
{
	if (n == -1)
	{
		perror(err);
		exit(1);
	}
	return n;
}

void cloudInit()
{
	connection.socketID = socket(AF_INET, SOCK_STREAM, 0);
//	connection.flags = errorCheck(fcntl(connection.socketID, F_GETFL), "could not get flags on TCP listening socket");
	errorCheck(fcntl(connection.socketID, F_SETFL, O_NONBLOCK), "could not set TCP listening socket to be non-blocking");

	memset(&connection.socketDestination, 0, sizeof(connection.socketDestination));              /* zero the struct */
	connection.socketDestination.sin_family = AF_INET;
	connection.socketDestination.sin_addr.s_addr = inet_addr(SERVER_IP);
	connection.socketDestination.sin_port = htons(SERVER_PORT);
}

int cloudConnect()
{
	rc_usleep(300000);
	//errorCheck(bind(connection.socketID, (struct sockaddr *) &connection.socketDestination, sizeof(struct sockaddr_in)), "could not bind");
	int conn = connect(connection.socketID, (struct sockaddr *)&connection.socketDestination, sizeof(struct sockaddr_in));
	if (0>conn)
	{
		connection.status = OFFLINE;
	    printf("\nCloud connection fail : %d \n", conn);
	} else {
		connection.status = ONLINE;
		printf("\nCloud-Robot connection established : %d \n", conn);
	}

	//errorCheck(listen(connection.socketID, 100), "could not listen");
	return EXIT_SUCCESS;
}

int readCloudCommand(char *command)
{
	printf("* %c %c %c %c %c * \n", command[1], command[2], command[3], command[4], command[5]);
	if ((command[2] != '{')
		|| (command[4] != '}')
		|| (command[5] != '!'))
	{
		return -1;
	}
	switch (command[3])
	{
		case 'i':
			motionDo(FORWARD);
			break;
		case 'j':
			motionDo(LEFT);
			break;
		case 'l':
			motionDo(RIGHT);
			break;
		case 'k':
			motionDo(STOP);
			break;
		case 'd':
			motionDo(HIGHTSPEED);
			break;
		case 's':
			motionDo(LOWSPEED);
			break;
		case 'z':
			motionDo(SERVO_LEFT);
			break;
		case 'x':
			motionDo(SERVO_RIGHT);
			break;
		case 'v':
			motionDo(SERVO_STOP);
			break;
		case '1':
			cameraSetPosition(CAMERA_LEFT);
			break;
		case '2':
			cameraSetPosition(CAMERA_HALF_LEFT);
			break;
		case '3':
			cameraSetPosition(CAMERA_CENTER);
			break;
		case '4':
			cameraSetPosition(CAMERA_HALF_RIGHT);
			break;
		case '5':
			cameraSetPosition(CAMERA_RIGHT);
			break;
		default:
			return -1;
			break;
	}
	return 0;
}

int cloudSendData(char *data)
{
   if (0 > send(connection.socketID, data, strlen(data) ,0))
   {
		connection.status = OFFLINE;
		printf("Error %s\n", strerror(errno));
		fflush(stdout);
		return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

int cloudWelcome()
{
	if (-1 != accept(connection.socketID, NULL, NULL))
	{
	  char msg[] = "hello\n";
	  printf("Got a connection; writing 'hello' then closing.\n");
	  send(connection.socketID, msg, sizeof(msg), 0);
	  close(connection.socketID);
	  return -1;
	}
	return 0;
}

int cloudReadData()
{
	int msgLen = recv(connection.socketID,&connection.buffer[0],CLOUD_BUFFER_SIZE,0);

	if(msgLen > 0) {
		printf(" %s", &connection.buffer[0]);
		//cloudSendData(&connection.buffer[0]);		// echo
		readCloudCommand(&connection.buffer[0]);	// read command
		return msgLen;
	}
	return 0;
}

int cloudTelemetryPost()
{
	// TODO: put all this on the telemetry.c file
	char *buffer;
	buffer = getTelemetryReport();
	cloudSendData(buffer);
	return EXIT_SUCCESS;
}

void cloudShutDown()
{
	close(connection.socketID);
}

Status getConnectionStatus()
{
	return connection.status;
}

void* __cloud_manager(__attribute__ ((unused)) void* ptr)
{
	cloudInit();
	//int timeToTelemetry = 0;
	while(rc_get_state()!=EXITING)
	{
		if (getConnectionStatus() == OFFLINE)
		{
			cloudConnect();
		} else {
			if (0 < cloudReadData())
			{
				// acknowledgement
			} else {
				cloudTelemetryPost();
			}
		}
		rc_usleep(1000000 / CLOUD_REFRESH_HZ);
	}
	return NULL;
}
