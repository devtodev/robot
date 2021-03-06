/*
 * utils.h
 *
 *  Created on: Aug 29, 2019
 *      Author: karl
 */

#ifndef JSON_H_
#define JSON_H_

void jsonBegin(char *result);
void jsonEnd(char *result);
void jsonObjBegin(char *name, char *result);
void jsonObjEnd(char *result);
void jsonKeyValue(char *key, char *value, char *result);
void jsonKeyDouble(char *key, double value, char *result);
void jsonKeyInt(char *key, int value, char *result);
void jsonKeyFloat(char *key, double value, char *result);

#endif /* JSON_H_ */
