/*
 * header.h
 *
 *  Created on: Oct 4, 2015
 *      Author: thanos
 */

#ifndef HEADER_H_
#define HEADER_H_
#include <cstring>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
const int MAXSIZE=32767;
const int MSS=1460;
using namespace std;

struct header {
		unsigned int seq_no;
		unsigned int ack_no;
		unsigned char ackflag;
		unsigned char finflag;
		unsigned short length;
		char data[MSS];

	header() {
		seq_no = 0;
		ack_no = 0;
		ackflag = 'F';
		finflag = 'F';
		length = 0;
		bzero(data, MSS);
	}

	void setValues(int seq, int ack, char flagvalue, char finvalue, string buffer) {
		seq_no = seq;
		ack_no = ack;
		ackflag = flagvalue;
		finflag = finvalue;
		if (ackflag == 'T')
			length = 0;
		else {
			length = buffer.length();
			memcpy(data, buffer.c_str(), buffer.length());
		}
	}
};
#endif /* HEADER_H_ */
