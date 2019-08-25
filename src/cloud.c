/*
 * cloud.c
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#include "telemetry.h"
#include "cloud.h"
#include "motion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXRCVLEN 500
#define PORTNUM 3000

#define MSG_WELCOME "Welcome to the jaguar house..."
#define SERVER_IP "192.168.7.1"
#define SERVER_PORT 3000

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

int cloudConnect()
{
	connection.socketID = socket(AF_INET, SOCK_STREAM, 0);
//	connection.flags = errorCheck(fcntl(connection.socketID, F_GETFL), "could not get flags on TCP listening socket");
	errorCheck(fcntl(connection.socketID, F_SETFL, O_NONBLOCK), "could not set TCP listening socket to be non-blocking");

	memset(&connection.socketDestination, 0, sizeof(connection.socketDestination));              /* zero the struct */
	connection.socketDestination.sin_family = AF_INET;
	connection.socketDestination.sin_addr.s_addr = inet_addr(SERVER_IP);
	connection.socketDestination.sin_port = htons(SERVER_PORT);

	//errorCheck(bind(connection.socketID, (struct sockaddr *) &connection.socketDestination, sizeof(struct sockaddr_in)), "could not bind");
	if (0<connect(connection.socketID, (struct sockaddr *)&connection.socketDestination, sizeof(struct sockaddr_in)))
	{
	   printf("Cloud connection fail");
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
		default:
			return -1;
			break;
	}
	return 0;
}

int cloudSendData(char *data)
{
   if (EXIT_SUCCESS != sendto(connection.socketID, data, strlen(data) ,0,
		      	  	  	  	  (struct sockaddr *) &connection.socketDestination,
							  sizeof connection.socketDestination))
   {
		 printf("ERROR: sending datagram message");
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

char *getConnectonBuffer()
{
	return connection.buffer;
}

int cloudTelemetryPost(Sensors sensors, Power power)
{
	cloudSendData("Hola!\n");
	return EXIT_SUCCESS;
}

void cloudShutDown()
{
	close(connection.socketID);
}

