/*
 * cloud.h
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#ifndef CLOUD_H_
#define CLOUD_H_

struct string
{
  char *ptr;
  size_t len;
};

int telemetryPost(Sensors sensors, Power power);

#endif /* CLOUD_H_ */
