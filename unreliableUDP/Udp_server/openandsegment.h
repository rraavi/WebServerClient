/*
 * openandsegment.h
 *
 *  Created on: Oct 4, 2015
 *      Author: thanos
 */

#ifndef OPENANDSEGMENT_H_
#define OPENANDSEGMENT_H_
#include "header.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

class openandsegment
{
public:
	int openandsegment1(int sock, struct header client_c, int wndw_sz, struct sockaddr_in client, socklen_t addr_size);
	char* reads( int i_byte, int f_byte, char *buffer,  FILE *fp);
};
;
#endif /* OPENANDSEGMENT_H_ */
