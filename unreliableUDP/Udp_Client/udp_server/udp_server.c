#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#define MAXSIZE 2000
#define MSS 1460

char *filename;
/*
 * structure to store the values that are parsed
 */
struct headerinfo
{
	char method[MAXSIZE];
	char uri[MAXSIZE];
	char version[MAXSIZE];
}h;
/*
 * error handling funtion
 */
void error(char *msg)
{
	perror(msg);
}
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

int split( int i_byte, int f_byte, char buffer[MSS],  FILE *fp, struct sockaddr_in client, socklen_t addr_size, int sock)
{
	char temp[MSS];
	int length=(f_byte-i_byte);
	int size_to_send;
	//cout<<"...Length of Data being Sent:"<<client_c.length<<"..."<<endl;
	fseek(fp, i_byte, SEEK_SET);
	fread(buffer, length,1, fp);
//	memcpy(temp,buffer,length);
	/*
	 * echo back to the client
	 */
	if((size_to_send = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&client, addr_size))<0)
	{
		error("CANNOT SEND 2\n");
		exit(0);
	}
	return 0;
}

int main(int argc, char **argv)
{
	int sock, fd;
	FILE *fp;
	struct sockaddr_in server, client;
	socklen_t addr_size;
	char *filename;
	char buff[MAXSIZE], buffer1[MAXSIZE];
	size_t file_len, size_to_send;
	char *token;
	char *t1;
	int segments;
	/*
	 * creating a streaming socket and check
	 */
	if((sock = socket(PF_INET, SOCK_DGRAM, 0))< 0)
	{
		error("ERROR IN SOCKET\n");
	    exit(0);
    	}
	/*
	 * build address/port structure
	 */
	 server.sin_family = AF_INET;
	 server.sin_port = htons(atoi(argv[1]));
	 server.sin_addr.s_addr = INADDR_ANY;
	 addr_size = sizeof(struct sockaddr);
	 /*
	  * bind the socket to a port address
	  */
	 if ((bind(sock, (struct sockaddr*)&server, addr_size))<0)
	 {
		 error("ERROR IN BINDING\n");
		 exit(0);
	 }
	 /*
	  * receive the details from client
	  */
	 if((recvfrom(sock, buffer1, sizeof(buffer1), 0, (struct sockaddr*)&client, &addr_size))<0)
	 {
		 error("ERROR IN RECEIVING : 1\n");
		 exit(0);
	 }
	 /*
	  * parse the details from request header
	  */
	 token = strtok(buffer1,"\r\n");
	 while (token!=NULL)
	 {
		 t1 = strstr(token,"GET");
		 if(t1!=NULL)
			 sscanf(t1, "%s %s %s", h.method, h.uri, h.version);
		 token = strtok(NULL, "\r\n");
	 }
	 filename=h.uri+1;
	 puts(filename);
	 if((fd = open(filename, O_RDONLY))<0)
	 {
    	 error("CANNOT OPEN THE FILE\n");
	 }
	 file_len = get_flength(fd);
 	 if(file_len<=MSS)
	 {
		segments=1;
		read(fd, buff, sizeof(buff));
		if((size_to_send = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&client, addr_size))<0)
		{
			error("CANNOT SEND 2:\n");
			exit(0);
		}
   	 }
	/*
	 * orelse segment the data into packets and send aacordingly
	 */
	else
	{
		int i;
		int i_byte=0, f_byte=MSS;
		segments = ((file_len/MSS)+1);
		if((fp=fopen(filename,"r"))<0)
		{
			error("CANNOT OPEN FILE:\n ");
			exit(0);
		}

		for(i=0;i<segments;i++)
		{
			split(i_byte,f_byte, buff,fp, client, addr_size, sock);
			i_byte=f_byte;
			f_byte=f_byte+MSS;
			if(f_byte>=file_len+MSS)
			{
				break;
			}
		}
	}
	close(sock);
	return 0;
}
