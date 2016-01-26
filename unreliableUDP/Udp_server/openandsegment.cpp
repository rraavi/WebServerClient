/*
 * openandsegment.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: thanos
 */

#include "openandsegment.h"
#include <cstring>
#include <map>

/*
 * set at the beginning of your file
 * lseek() returns number of byte measured
 * set again at the beginning of the file
 */
size_t  get_flength(int fd)
{
        size_t  len;
        lseek(fd, 0, SEEK_SET);
        len = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        return len;
}


int setinit_values(struct header client_c)
{
	client_c.seq_no=0;
	client_c.ack_no=0;
	client_c.ackflag=0;
	return 0;
}
struct timeval tim;

/*
 *  segment the file, send seq_no and recieve the ack's. maps to implement the window
 */
int openandsegment::openandsegment1(int sock,struct header client_c, int wndw_sz, struct sockaddr_in client, socklen_t addr_size)
{
	map<int, header>ackTrack;
	int fd, segments;
	FILE *fp;
	char buff[MAXSIZE];
	setinit_values(client_c);
	size_t file_len, size_to_send;
	if((fp=fopen(client_c.data,"r"))<0)
	{
		cerr << "CANNOT OPEN FILE: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}
	fd = open(client_c.data, O_RDONLY);
	file_len = get_flength(fd);
	cout<<"...Number of Bytes are:"<<file_len<<"..."<<endl;
	if((sendto(sock, &file_len, sizeof(file_len), 0, (struct sockaddr*)&client, addr_size))<0)
    {
		cerr << "CANNOT SEND 1: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
    }
	/*
	 * in case of one MSS, send the entire data
	 */
	if(file_len<=MSS)
	{
		segments=1;
		read(fd, buff, sizeof(buff));
		strcpy(client_c.data,buff);
		client_c.length=file_len;
		client_c.seq_no = client_c.ack_no + 1;
		if((size_to_send = sendto(sock, &client_c, sizeof(header), 0, (struct sockaddr*)&client, addr_size))<0)
		{
			cerr << "CANNOT SEND 2: " << strerror( errno ) << endl;
			exit( EXIT_FAILURE );
		}
		cout<<"...Details Sent With seq_no:"<<client_c.seq_no<<"..."<<endl;
		if((recvfrom(sock, &client_c, sizeof(header), 0, (struct sockaddr*)&client, &addr_size))<0)
		{
			cerr << "CANNOT RECIEVE 1: " << strerror( errno ) << endl;
			exit( EXIT_FAILURE );
	    }
		cout<<"...Ack Received:"<<client_c.ack_no<<"..."<<endl;
   	}
	/*
	 * orelse segment the data into packets and send aacordingly
	 */
	else
	{
		int count=0;
		segments = ((file_len/MSS)+1);
		cout<< "the number of segments are:"<<segments<<endl;
		int i_byte=0, f_byte=MSS;
		read(fd, buff, sizeof(buff));
		map<int, header>ackTrack;
		do
		{
			double t1=0,t2=0;
			for( int j=0;j<wndw_sz;j++)								//sends the seqno's
			{
					header client_c=header();
					memcpy(client_c.data,reads(i_byte,f_byte,buff,fp),1460);
					client_c.seq_no=i_byte;
					client_c.ack_no=f_byte;
					if(sendto(sock, &client_c, sizeof(client_c), 0, (struct sockaddr*)&client, addr_size)==-1)
					{
						cerr<<"ERROR"<<strerror(errno)<<endl;
					}

					gettimeofday(&tim, NULL);
					t1=tim.tv_sec+(tim.tv_usec/1000000.0);
					cout<<"...Packet Sent..."<<client_c.seq_no<<endl;
					ackTrack[client_c.seq_no]=client_c;
					i_byte=f_byte;
					f_byte=f_byte+MSS;
					if (f_byte>=file_len+MSS)
						break;
			}
			cout<<"Packets sent within window"<<endl;
		count=0;											//waits for the ack's
		while (!ackTrack.empty())
		{
			if(count==wndw_sz)
				break;
			if((recvfrom(sock, &client_c, sizeof(header), 0, (struct sockaddr*)&client, &addr_size))<0)
			{
				cerr << "CANNOT RECIEVE 1: " << strerror( errno ) << endl;
				exit( EXIT_FAILURE );
			}
			cout<<"...Ack Received:"<<client_c.ack_no<<"..."<<endl;
			gettimeofday(&tim, NULL);
			t2=tim.tv_sec+(tim.tv_usec/1000000.0);
			cout<<"***RTT:"<<(t2-t1)<<"seconds elapsed***"<<endl;
			ackTrack.erase(client_c.seq_no);
			count++;
			cout<<"HERE"<<endl;
		}
		}while(f_byte<=file_len);
	}
	return 0;
}
/*
 * Split the file and reads it into a buffer
 */
char* openandsegment::reads( int i_byte, int f_byte, char *buffer,  FILE *fp)
{

	char *temp;
	int length=(f_byte-i_byte);
	//cout<<"...Length of Data being Sent:"<<client_c.length<<"..."<<endl;
	fseek(fp, i_byte, SEEK_SET);
	fread(buffer, i_byte,length, fp);
	memcpy(temp,buffer,length);
	return temp;
}
