/*
 * cloud.c
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#include "telemetry.h"
#include "cloud.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXRCVLEN 500
#define PORTNUM 3000

#define MSG_WELCOME "Welcome to the jaguar house..."

int main(int argc, char *argv[])
{
   char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   int len, mysocket;
   struct sockaddr_in dest;

   mysocket = socket(AF_INET, SOCK_STREAM, 0);

   memset(&dest, 0, sizeof(dest));                   /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = inet_addr("192.168.0.19"); /* set destination IP number - localhost, 127.0.0.1*/
   dest.sin_port = htons(PORTNUM);                   /* set destination port number */

   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

   if (sendto(mysocket,MSG_WELCOME, sizeof MSG_WELCOME ,0,
         (struct sockaddr *) &dest,sizeof dest) == -1)
         perror("sending datagram message");

   len = recv(mysocket, buffer, MAXRCVLEN, 0);
   buffer[len] = '\0';
   printf("Received %s (%d bytes).\n", buffer, len);

   close(mysocket);
   return EXIT_SUCCESS;
}

int telemetryPost(Sensors sensors, Power power)
{

  return 0;
}
