/*
 * cloud.c
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens


*/

#include "cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "action.h"
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "rc/time.h"
#include "rc/start_stop.h"
#define ADDRESS     "tcp://test.mosquitto.org:1883"
#define CLIENTID    "Robot01"
#define TOPIC_TELEMETRY       "robot/telemetry"
#define TOPIC_MOTOR		        "robot/motor"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;
int connState = OFFLINE;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

void readCommand(char *command)
{
	if (strcmp(command, "MOVE_FORWARD")==0) addAction(MOVE_FORWARD);
	if (strcmp(command, "MOVE_LEFT")==0) addAction(MOVE_LEFT);
	if (strcmp(command, "MOVE_RIGHT")==0) addAction(MOVE_RIGHT);
	if (strcmp(command, "MOVE_STOP")==0) addAction(MOVE_STOP);
	if (strcmp(command, "HIGHTSPEED")==0) addAction(HIGHTSPEED);
	if (strcmp(command, "LOWSPEED")==0) addAction(LOWSPEED);
	if (strcmp(command, "CAMERA_LEFT") ==0) addAction(CAMERA_LEFT);
	if (strcmp(command, "CAMERA_HALF_LEFT")==0) addAction(CAMERA_HALF_LEFT);
	if (strcmp(command, "CAMERA_CENTER")==0) addAction(CAMERA_CENTER);
	if (strcmp(command, "CAMERA_HALF_RIGHT")==0) addAction(CAMERA_HALF_RIGHT);
	if (strcmp(command, "CAMERA_RIGHT")==0)
		{
		addAction(CAMERA_RIGHT);
		}
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    readCommand(message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
	connState  = OFFLINE;
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}



void* __mqtt_manager(__attribute__ ((unused)) void* ptr)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    MQTTClient_deliveryToken token;

	while(rc_get_state()!=EXITING)
	{
		   while (connState == OFFLINE)
		    {
			   if (((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS))
			   {
				   printf("Failed to connect, return code %d\n", rc);
			   } else {
				   connState = ONLINE;
				   printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n Press Q<Enter> to quit\n\n", TOPIC_TELEMETRY, CLIENTID, QOS);
				   MQTTClient_subscribe(client, TOPIC_MOTOR, QOS);
			   }
			   rc_usleep(10000000 / CLOUD_REFRESH_HZ);
		    }
		MQTTClient_message pubmsg = MQTTClient_message_initializer;

	    pubmsg.payload = getTelemetryReport();
	    pubmsg.payloadlen = strlen(pubmsg.payload);
	    pubmsg.qos = QOS;
	    pubmsg.retained = 0;
	    MQTTClient_publishMessage(client, TOPIC_TELEMETRY, &pubmsg, &token);

		rc_usleep(1000000 / CLOUD_REFRESH_HZ);
    }
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    // return rc;
    return NULL;
}
