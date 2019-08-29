/*
 * utils.c
 *
 *  Created on: Aug 29, 2019
 *      Author: karl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void jsonBegin(char *result)
{
	strcpy(result, "{\0");
}

void jsonEnd(char *result)
{
	int i = strlen(result);
	result[i - 2] = '\0';
	strcat(result, "}\0");
}

void jsonObjBegin(char *name, char *result)
{
	strcat(result, "\"\0");
	strcat(result, name);
	strcat(result, "\" : { \0");
}
void jsonObjEnd(char *result)
{
	int i = strlen(result);
	result[i - 2] = '\0';
	strcat(result, "}, \0");
}

void jsonKeyValue(char *key, char *value, char *result)
{
	strcat(result, "\"\0");
	strcat(result, key);
	strcat(result, "\" : \"\0");
	strcat(result, value);
	strcat(result, "\", \0");
}

void jsonKeyDouble(char *key, double value, char *result)
{
	char valueBuffer[10];
	snprintf(valueBuffer, 10, "%4.1f", value);
	jsonKeyValue(key, valueBuffer, result);
}

void jsonKeyInt(char *key, int value, char *result)
{
	char valueBuffer[10];
	snprintf(valueBuffer, 10, "%d", value);
	jsonKeyValue(key, valueBuffer, result);
}


void jsonKeyFloat(char *key, double value, char *result)
{
	char valueBuffer[10];
	snprintf(valueBuffer, 10, "%4.1f", value);
	jsonKeyValue(key, valueBuffer, result);
}
