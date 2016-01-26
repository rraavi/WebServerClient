#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define MAXSIZE 2000

/*struct httpresponse
{
	char type[MAXSIZE];
	char version[MAXSIZE];
	char status[MAXSIZE];
}r;*/
	char *filename;
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

void parseheader(char buffer1[])
{
	char *token;
	char *t1;
	token = strtok(buffer1,"\r\n");
	while (token!=NULL)
	{
		if(strcmp(token,"GET"))
			sscanf(buffer1, "%s %s %s", h.method, h.uri, h.version);
		t1=strstr(token,"Connection");
		if(t1 != NULL)
			sscanf(t1, "Connection: %s", h.conntype);
		token = strtok(NULL, "\r\n");
	}
	printf("request parsed");
}	

int httpresponse(char *filename, int newsockfd)
{
	FILE *fp;
	char buf[1000];
	int sz;
	int total;
	char line1[MAXSIZE];
	char line2[MAXSIZE];
	char line3[MAXSIZE];
	char finalline[MAXSIZE];
	char *reply;
	filename = h.uri+1;
	fp = fopen(filename,"r");
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	fread(buf, 1, sz, fp);
	if(strcmp(h.conntype, "keep-alive"))
	{
		if(fp!=NULL)
		{
			reply = "HTTP/1.1 200 OK\n"
					"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
					"Server: Apache/2.2.3\n"
					"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
					"ETag: \"56d-9989200-1132c580\"\n"
					"Content-Type: text/html\n"
					"Accept-Ranges: bytes\n"
					"Connection: close\n"
					"\n";
/*	strcat(line1, "HTTP/1.1 200 OK \n"
                 "Date: Thu, 19 Feb 2009 12:27:04 GMT \n"
                 "Server: Apache/2.2.3 \n"
                 "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT \n"
                 "ETag: \"56d-9989200-1132c580\"\n"
                 "Content-Type: text/html \n"
                 "Content-Length: %i \n"
                 "Accept-Ranges: bytes \n"
                 "Connection: close \n"

                         "\n");*/
		//strcat(line2, "Content-type: text/html\n");
			strcat(line3, buf);
			strcat(finalline, reply);
		//strcat(finalline, line2); 
			strcat(finalline, line3);
		//total=strlen(finalline);
			send(newsockfd, finalline, strlen(finalline), 0);
		//write(newsockfd, finalline,total);
		}
		else
		{
			strcat(line1, "HTTP/1.1 404 Not Found\n");
			strcat(line2, "Content-type: text/html\n");
			strcat(line3, "<html><title>FileNotFound</title>\n <body>\n 404, FileNotFound \n </body>\n </html>\n");
			strcat(finalline, reply);
			strcat(finalline, line3);
			send(newsockfd, finalline, strlen(finalline), 0);
		}
	}
	else if(strcmp(h.conntype,"close"))
	{
		if(fp!=NULL)
		{
			reply = "HTTP/1.1 200 OK\n"
					"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
					"Server: Apache/2.2.3\n"
					"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
					"ETag: \"56d-9989200-1132c580\"\n"
					"Content-Type: text/html\n"
					"Accept-Ranges: bytes\n"
					"Connection: close\n"
					"\n";
			strcat(line3, buf);
			strcat(finalline, reply);
			strcat(finalline, line3);
		//total=strlen(finalline);
			send(newsockfd, finalline, strlen(finalline), 0);
		//write(newsockfd, finalline,total);
		}
		else
		{
			strcat(line1, "HTTP/1.1 404 Not Found\n");
			strcat(line2, "Content-type: text/html\n");
			strcat(line3, "<html><title>FileNotFound</title>\n <body>\n 404, FileNotFound \n </body>\n </html>\n");
			strcat(finalline, reply);
			strcat(finalline, line3);
			send(newsockfd, finalline, strlen(finalline), 0);			
		}
	}
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, pno, clen, n;
	char buffer[MAXSIZE];
	struct sockaddr_in s_addr, c_addr; //structure is defined in <netinet/in.h> 
	size_t nread;
	FILE *fp;
	int i;

	if(argc<2)
		perror("NO PORT ADDRESS GIVEN");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
		perror("ERROR OPENING SOCKET");
	bzero((char*)&s_addr,sizeof(s_addr));
	pno = atoi(argv[1]);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(pno);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd,(struct sockaddr *) &s_addr, sizeof(s_addr))<0)
		perror("ERROR ON BINDING");
	listen(sockfd,5);
	clen = sizeof(c_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &c_addr, &clen);
	if(newsockfd<0)
		perror("ERROR ON ACCEPTING");
	bzero(buffer,2000);
	n = read(newsockfd,buffer,2000);
	if(n<0)
		perror("ERROR READING YOUR MESSAGE");
	printf("The message is :\n%s\n",buffer);
	//n = write(newsockfd,"MESSAGE RECIEVED",16);
	parseheader(buffer);
	httpresponse(filename, newsockfd);
	if(n<0)
		perror("ERROR WRITING TO THE SOCKET");
	close(sockfd);
	close(newsockfd);
}

