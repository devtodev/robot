/*
 * bluetooth.c
 *
 *  Created on: Mar 28, 2020
 *      Author: karl
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "btLayer.h"

#define BTINQUIRYMAX	255


int btFindDevice(char *deviceAddrToFind)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = BTINQUIRYMAX;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);

        if (strcmp(addr, deviceAddrToFind) == 0)
        {
            free( ii );
            close( sock );
        	return 1;
        }
    }

    free( ii );
    close( sock );
    return 0;
}

int btInit()
{
	if (btFindDevice(BTCOMPUTERADDR))
	{
		printf("connecting with BT computer interface...");
		// send a connection request
	} else {
		printf("BT computer interface not accessible...");
	}
	return 0;
}
