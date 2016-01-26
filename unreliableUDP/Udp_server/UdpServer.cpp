/*
 * UdpServer.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: thanos
 */


#include "openandsegment.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[])
{
	cout<<"...Starting the Server..."<<endl;
	int sock;
	int wndw_sz;
	header client_c;
	struct sockaddr_in server, client;
	socklen_t addr_size;
	//char buffer1[MSS];
	if(argc<3)
	{
		cerr << "**Usage: " << argv[ 0 ] << "Port	WindowSize**" << endl;
	    exit(EXIT_FAILURE);
	}
	/*
	 * creating a streaming socket and check
	 */
	if((sock = socket(PF_INET, SOCK_DGRAM, 0))< 0)
	{
		cerr << "ERROR IN SOCKET: " << strerror( errno ) <<endl;
		exit( EXIT_FAILURE );
	}
	cout<<"...Socket Created And Opened..."<<endl;
	/*
	 * build address/port structure
	 */
	 server.sin_family = AF_INET;
	 server.sin_port = htons(atoi(argv[1]));
	 server.sin_addr.s_addr = INADDR_ANY;
	 addr_size = sizeof(struct sockaddr);
	 wndw_sz = atoi(argv[2]);
	 /*
	  * bind the socket to a port address
	  */
	 if ((bind(sock, (struct sockaddr*)&server, addr_size))<0)
	 {
			cerr << "ERROR IN BINDING: " << strerror( errno ) << endl;
			exit( EXIT_FAILURE );
	 }
	 cout<<"...Server Address Binding Done..."<<endl;
	 /*
	  * receive the details from client
	  */
	 if((recvfrom(sock, &client_c, sizeof(header), 0, (struct sockaddr*)&client, &addr_size))<0)
	 {
			cerr << "ERROR IN RECEIVING: " << strerror( errno ) << endl;
			exit( EXIT_FAILURE );
	 }

	 cout<<"...The Filename is: "<<client_c.data<<"..."<<endl;
	 openandsegment obj;
	 obj.openandsegment1(sock,client_c, wndw_sz, client, addr_size);

	 return 0;
	 close(sock);
}
