/*
 * bluetooth.h
 *
 *  Created on: Mar 28, 2020
 *      Author: karl
 */

#ifndef SRC_BLUETOOTH_H_
#define SRC_BLUETOOTH_H_

#define BTCOMPUTERADDR	"12:12:12:12"

int btFindDevice(char *deviceAddrToFind);
int btInit();

#endif /* SRC_BLUETOOTH_H_ */
