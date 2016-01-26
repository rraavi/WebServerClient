/*
 * udp_client.c
 * Created on: Sep 16, 2015
 * 	   Author: Raghu
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/*
 * error handling function
 */
void error(char *msg)
{
	perror(msg);
}

int main(int argc, char **argv)
{
	int pno, by_rv, n;
	int sockfd;
	struct sockaddr_in s_addr;
	socklen_t addrsize = sizeof(struct sockaddr);
	char buffer_r[2000];
	char buffer_s[2000];
	char *filename;
	size_t sztord = 0;
	size_t szrd =0;
	pno = atoi(argv[1]);
	/*
	 * checks for the number of arguments being passed
	 */
	if(argc<4)
	{
		printf("Usage: <%s><Address><pno><filename>\n", argv[0]);
	    exit(0);
	}
	/*
	 * open a socket and check if it is opened properly
	 */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		error("ERROR IN SOCKET\n");
		exit(0);
	}
	/*
	 * build address/port structure
	 */
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(atoi(argv[2]));
    s_addr.sin_addr.s_addr = inet_addr(argv[1]);
    filename = argv[3];
	sprintf(buffer_s,"GET /%s HTTP/1.1\r\nHost: \r\n\r\n", filename);
	/*
	 * echo/send the details to the server
	 */
	n = sendto(sockfd, buffer_s, strlen(buffer_s), 0, (struct sockaddr*)&s_addr, addrsize);
	printf("Details sent\n");
	if(n<0)
	{
		error("ERROR SENDING TO THE SOCKET");
		exit(0);
	}
	if((recvfrom(sockfd, (size_t*)&sztord, sizeof(sztord), 0, (struct sockaddr*)&s_addr, &addrsize))<0)
	{
		error("ERROR IN RECEIVING");
	    exit(0);
	}
	/*
	 * error checking for size_to_read
	 */
	if (sztord < 0)
	{
		error("ERROR TO READ < 0\n");
		exit(0);
	}
	/*
	 * receive the data from the udp server
	 */
	if((szrd = recvfrom(sockfd, buffer_r, sizeof(buffer_r), 0, (struct sockaddr*)&s_addr, &addrsize))<0)
	{
		error("ERROR WHILE RECEIVING");
	    exit(0);
	}
    printf("%s", buffer_r);
    /*
     * prints the number of bytes and the last byte sent
     */
	printf("The Last Byte has been Recieved\n Number of bytes: %lu\n", sztord);
    close(sockfd);
	return 0;

}
