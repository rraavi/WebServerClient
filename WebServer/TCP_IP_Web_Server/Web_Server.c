/*
 * web_server.c
 *
 *  Created on: Sep 17, 2015
 *      Author: Raghu
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXSIZE 2000

char *filename;
/*
 * structure to store the header requests
 */
struct headerinfo
{
	char method[MAXSIZE];
	char uri[MAXSIZE];
	char version[MAXSIZE];
	char conntype[MAXSIZE];
}h;

void error(char *msg)
{
	perror(msg);
}
/*
 *function parses the details into the structure
 *function checks for the persistence and sets the flag accordingly
 */
int parseheader(char buffer1[])
{
	char *token;
	char *t1;
	char *t2;
	int flag;
	/*
	 * The Flag specifies the connection type, flag = 1 for Persistent and flag = 0 for Non persistent connections
	 */
	token = strtok(buffer1,"\r\n");
	while (token!=NULL)
	{
		t1 = strstr(token,"GET");
		if(t1!=NULL)
			sscanf(t1, "%s %s %s", h.method, h.uri, h.version);
		t2=strstr(token,"Connection");
		if(t2!= NULL)
			sscanf(t2, "Connection: %s", h.conntype);
		token = strtok(NULL, "\r\n");
	}
	if(strncmp(h.conntype, "keep-alive", strlen(h.conntype))==0)
		flag=1;
	else
		flag=0;
	printf("request parsed \n");
	return flag;
}
/*
 * handles bad request
 */
int badrequest(int newsockfd)
{
	int n;
	char line1[MAXSIZE] = {0};
	strcat(line1, "HTTP/1.1 400 Bad Request\n");
	strcat(line1, "Content-Type: text/html; charset=utf-8\n");
	strcat(line1, "Connection: keep-alive\n");
	strcat(line1, "Content-Length:5 \n\n");
	strcat(line1, "<html><title>BadRequest</title>\n <body>\n 400, BadRequest \n </body>\n </html>");
	n = send(newsockfd, line1, strlen(line1), 0);
	if(n<0)
			perror("ERROR WRITING TO THE SOCKET");

	return 0;
}
/*
 * builds and sends the response back
 */
int httpresponse(char *filename, int newsockfd)
{
	FILE *fp;
	char buf[1000]={0};
	int sz;
	int n;
	char line1[MAXSIZE] = {0};
	char line2[MAXSIZE] = {0};
	char line3[MAXSIZE] = {0};
	char line4[MAXSIZE] = {0};
	char line5[MAXSIZE] = {0};
	char line6[MAXSIZE] = {0};
	char line7[MAXSIZE] = {0};
	char finalline[MAXSIZE] = {0};
	filename = h.uri+1;
	puts(filename);
	fp = fopen(filename,"r");
	if(fp!=NULL)
	{
		fseek(fp, 0L, SEEK_END);
		sz = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		fread(buf, 1, sz, fp);
		puts(h.conntype);
		if(strncmp(h.conntype, "keep-alive", strlen(h.conntype))==0)
		{
			printf("THE TYPE THAT IS BEING HANDLED IS FOR PERSISTENT CONNECTIONS");
			strcat(line1, "HTTP/1.1 200 OK\n");
			strcat(line2, "Content-Type: text/html; charset=utf-8\n");
			strcat(line3, "Connection: keep-alive\n");
			strcat(line4, "Content-Length:");
			sprintf(line5,"%d", sz);
			strcat(line6,"\n\n");
			strcat(line7, buf);
			strcat(finalline, line1);
			strcat(finalline, line2);
			strcat(finalline, line3);
			strcat(finalline, line4);
			strcat(finalline, line5);
			strcat(finalline, line6);
			strcat(finalline, line7);
			/*
			 * echo back anything
			 */
			n = send(newsockfd, finalline, strlen(finalline), 0);
			if(n<0)
					perror("ERROR WRITING TO THE SOCKET");
		}
		else
		{
			printf("THE TYPE THAT IS BEING HANDLED IS FOR NON-PERSISTENT CONNECTIONS");
			strcat(line1, "HTTP/1.0 200 OK\n");
			strcat(line2, "Content-Type: text/html; charset=utf-8\n");
			strcat(line3, "Connection: close\n");
			strcat(line4, "Content-Length:");
			sprintf(line5, "%d", sz);
			strcat(line6,"\n\n");
			strcat(line7, buf);
			strcat(finalline, line1);
			strcat(finalline, line2);
			strcat(finalline, line3);
			strcat(finalline, line4);
			strcat(finalline, line5);
			strcat(finalline, line6);
			strcat(finalline, line7);
			/*
			 * echo back anything
			 */
			n = send(newsockfd, finalline, strlen(finalline), 0);
			if(n<0)
			perror("ERROR WRITING TO THE SOCKET");
		}
	}
	else
	{
		strcat(line1, "HTTP/1.1 404 Not Found\r\n");
		strcat(line2, "Content-Type: text/html; charset=utf-8\n Connection: close\n Content-Length:20 \n\n");
		strcat(line3, "<html><title>FileNotFound</title>\n <body>\n 404, FileNotFound \n </body>\n </html>");
		strcat(finalline, line1);
		strcat(finalline, line2);
		strcat(finalline, line3);
		/*
		 * echo back
		 */
		n = send(newsockfd, finalline, strlen(finalline), 0);
		if(n<0)
			perror("ERROR WRITING TO THE SOCKET");
	}
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, pno, clen, n, keep_alive=1,flag_r_value;
	char buffer[MAXSIZE]={0};
	struct sockaddr_in s_addr, c_addr;
	/*
	 * structure is defined in <netinet/in.h>
	 */
	FILE *fp;
	if(argc<2)
	{
		perror("NO PORT ADDRESS GIVEN");
		exit(0);
	}
	/*
	 * creating the socket
	 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("ERROR OPENING SOCKET");
		exit(0);
	}
	/*
	 * address/port structure
	 */
	bzero((char*)&s_addr,sizeof(s_addr));
	pno = atoi(argv[1]);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(pno);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*
	 * assign a port number
	 */
	if(bind(sockfd,(struct sockaddr *) &s_addr, sizeof(s_addr))<0)
	{
		perror("ERROR ON BINDING");
		exit(0);
	}

	while(1)
	{
		/*
		 * make it listening socket
		 */
		listen(sockfd,5);
		clen = sizeof(c_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &c_addr, &clen);
		if(newsockfd<0)
		{
			perror("ERROR ON ACCEPTING");
			exit(0);
		}
		bzero(buffer,2000);
		n = read(newsockfd,buffer,2000);
		if(n<0)
		{
			perror("ERROR READING YOUR MESSAGE");
			close(newsockfd);
			continue;
		}
		printf("The message is :\n%s\n",buffer);
		/*
		 * the flag value returned at parse header is stored here
		 */
		flag_r_value = parseheader(buffer);
		if(strcmp(h.method,"GET")!=0)
		{
			badrequest(newsockfd);
		}
		if(flag_r_value==1)
		{
			while(keep_alive)
			{
				httpresponse(filename, newsockfd);
				bzero(buffer,2000);
				n = read(newsockfd,buffer,2000);
				parseheader(buffer);
				if(flag_r_value==0)
				{
					keep_alive=0;
				}
			}
			/*
			 * close data connection
			 */
			close(newsockfd);
		}
		else
		{
			httpresponse(filename, newsockfd);
			/*
			 * close data connection
			 */
			close(newsockfd);
		}
	}
	close(sockfd);
	return 0;
}
