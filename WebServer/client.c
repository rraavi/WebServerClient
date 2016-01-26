#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
	perror(msg);
}

int main(int argc, char *argv[])
{
	int sockfd, pno, n;
	char filename[100], conntype[100];
	struct sockaddr_in s_addr;
	struct hostent *server;
	char buffer[2000];
	if (argc < 5) 
	{
		fprintf(stderr,"enter %s Hostaddr Portno Filename Connection-Type\n",argv[0]);
	}
	pno = atoi(argv[2]);
	strcpy(filename,argv[3]);
	puts(filename);
	strcpy(conntype,argv[4]);
	puts(conntype);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("ERROR WHILE OPENING SOCKET");
	}
//server = gethostbyaddr(argv[1]);

/*if(sizeof(s_addr.sin_addr.s_addr)==0)
{
fprintf(stderr,"ERROR: THERE'S NO SUCH ADDRESS \n");
}*/
	bzero((char *) &s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&s_addr.sin_addr.s_addr, server->h_length);
	s_addr.sin_port = htons(pno);
	s_addr.sin_addr.s_addr = htons(atoi(argv[1]));
	if(connect(sockfd,(struct sockaddr *)&s_addr, sizeof(s_addr))<0)
	{
		error("ERROR WHILE CONNECTING");
	}
	bzero(buffer,2000);
	printf("Sending the details to the server:");
	sscanf(buffer,"GET /%s HTTP/1.1\r\nHost: \r\nConnection: %s\r\n\r\n", filename, conntype); 
	n = write(sockfd, buffer, strlen(buffer));
	if(n<0)
	{
		error("ERROR READING FROM THE SOCKET");
	}
	printf("%s\n",buffer);
	close(sockfd);
	return 0;
}