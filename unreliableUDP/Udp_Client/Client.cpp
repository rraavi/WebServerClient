/*
 * Client.cpp
 * Created on: Oct 2, 2015
 * 	   Author: Raghu
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "header.h"
#include<fstream>
#include <map>
#include <queue>
using namespace std;

int writer(header client_c)
{
	ofstream myfile ("dum.txt", ios::app);
	if(client_c.finflag=='T')
	{
		myfile.close();
	}
	if (myfile.is_open())
	{
	    myfile.write(client_c.data,client_c.length);
	    myfile.close();
	}
	else
	{
		cerr<<"ERROR"<<strerror(errno)<<endl;
	}
	return 0;
}

void threeDuplicateAcks(int sockfd,header client_c,struct sockaddr_in s_addr,socklen_t addrsize)
{
	int i = 0;
	client_c.seq_no=0;
	while (i < 3)
	{
		 if((sendto(sockfd, &client_c, sizeof(header), 0, (struct sockaddr*)&s_addr, addrsize))<0)
		 {
			 cerr << "ERROR SENDING:" << strerror( errno ) << endl;
			 exit( EXIT_FAILURE );
		 }
		i++;
	}
}

int main(int argc, char **argv)
{
	queue<struct header> myqueue;
	int n;
	int wndw_sz,no_pckts;
	int sockfd;
	header client_c;
	//File Descriptor for our Socket
	struct sockaddr_in s_addr;
	socklen_t addrsize = sizeof(struct sockaddr);
//	char buffer_r[1500];
//	char buffer_s[1500];
//	char *filename;
	size_t sztord = 0;
	size_t szrd =0;
	//pno = atoi(argv[1]);
	/*
	 * checks for the number of arguments being passed
	 */
	if(argc<5)
	{
		cerr << "**Usage: " << argv[ 0 ] << " Address 	Port 	FileName 	WindowSize**" << endl;
	    exit(EXIT_FAILURE);
	}
	/*
	 * open a socket and check if it is opened properly
	 */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		cerr << "socket: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}
	/*
	 * build address/port structure
	 */
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(atoi(argv[2]));
    s_addr.sin_addr.s_addr = inet_addr(argv[1]);
    strcpy(client_c.data,argv[3]);
    wndw_sz=atoi(argv[4]);
//    filename = argv[3];
//	bzero(buffer_s, 1500);
//	sprintf(buffer_s,"%s\r\n",filename);

	/*
	 * echo/send the details to the server
	 */
	n = (sendto(sockfd, &client_c, sizeof(header), 0, (struct sockaddr*)&s_addr, addrsize));
	cout<<"**Details Sent**\n"<<endl;
	if(n<0)
	{
		cerr << "ERROR WHILE READING SOCKET: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}
	if((recvfrom(sockfd, (size_t*)&sztord, sizeof(sztord), 0, (struct sockaddr*)&s_addr, &addrsize))<0)
	{
		cerr << "ERROR IN RECEIVING: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}
	cout<<"**Total Number Of Bytes Have Been Received**\n"<<endl;
	/*
	 * error checking for size_to_read
	 */
	if (sztord==0)
	{
		cerr << "ERROR TO READ: " << strerror( errno ) << endl;
		exit( EXIT_FAILURE );
	}
	/*
	 * receive the data from the udp server
	 */
	bool finAck = false;
	while (!finAck)
	{
		map<int,header> seqTrack;
		queue<int>ackTrack;
		int count = 0;
		while (count < wndw_sz)
		{
			cout<<"inside while"<<endl;
			int	bytes=(recvfrom(sockfd, &client_c, sizeof(header), 0, (struct sockaddr*)&s_addr, &addrsize));
			if(bytes<0)
			{
				cerr << "ERROR IN RECEIVING: " << strerror( errno ) << endl;
				exit( EXIT_FAILURE );
			}
			cout<<"length"<<endl;
			//client_c.data[client_c.length]='\0';
			cout<<"length"<<endl;
			int temp_len=client_c.length;
			cout<<"**Sequence Number is:"<<client_c.seq_no<<"**"<<endl;
			seqTrack[client_c.seq_no]=client_c;
			ackTrack.push(client_c.seq_no);
			if (client_c.finflag == 'T')
			{
				while (!ackTrack.empty())
				{
					header ack = header();
					int temp = ackTrack.front();
					int ack_no = temp + MSS;
					ackTrack.pop();
					if (ackTrack.empty())
						ack.setValues(temp,temp+temp_len,'T','F',"");
					else
						ack.setValues(temp,ack_no,'T','F',"");
					memset(ack.data,0,1460);
					if((sendto(sockfd, &ack, sizeof(header), 0, (struct sockaddr*)&s_addr, addrsize))<0)
					{
						cerr << "ERROR SENDING: " << strerror( errno ) << endl;
						exit( EXIT_FAILURE );
					}
				}
				cout<<"**Seq_no Received and Ack_no Sent Back"<<endl;
				finAck = true;
				break;
			}
			count++;
		}
		count=0;
		while (count<wndw_sz)
		{
			header ack = header();
			if (!finAck)
			{
				if (ackTrack.empty())
					ack.setValues(0,MSS,'T','F',"");
				else
				{
					int seq_no = ackTrack.front();
					int ack_no = seq_no + MSS;
					ack.setValues(seq_no,ack_no,'T','F',"");
				}
				memset(ack.data,0,1460);
				ackTrack.pop();
			}
			if((sendto(sockfd, &ack, sizeof(header), 0, (struct sockaddr*)&s_addr, addrsize))<0)
			{
				cerr << "ERROR SENDING:" << strerror( errno ) << endl;
				exit( EXIT_FAILURE );
			}
			cout<<ack.ack_no<<endl;
			count++;
		}
		writer(client_c);

		/*
		 * prints the number of bytes and the last byte sent
		 */
		cout<<"packets reached within window size"<<endl;

		//exit(0);
	}
	cout<<"***The Last Byte Has Been Recieved & Number Of Bytes:"<<sztord<<"***"<<endl;

//	close(sockfd);
return 0;
}
