#include        <sys/socket.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <arpa/inet.h>
#include        <sys/types.h>
#include        <string.h>
#include        <unistd.h>

#define ECHOMAX 255
#define BACKLOG 128

void
DieWithError(const char *errorMessage)
{
        perror(errorMessage);
        exit(1);
}

void sendMessage(FILE *fp, int sockfd)
{
	ssize_t n;
        char    sendline[ECHOMAX], recvline[ECHOMAX];

        while (fgets(sendline, ECHOMAX, fp) != NULL) {

               write(sockfd, sendline, strlen(sendline));

//                if ( (n = read(sockfd, recvline, ECHOMAX)) == 0)
//                        DieWithError("str_cli: server terminated prematurely");
//			recvline[ n ] = '\0';
//                fputs(recvline, stdout);
        }
}

void
EchoString(int sockfd)
{
    ssize_t n;
    char    line[ECHOMAX],recvline[ECHOMAX];

    for ( ; ; ) {
	    if ( (n = read(sockfd, line, ECHOMAX)) == 0 )
   	    	return; 
            printf("mesg = %s\n",line );
    //fputs(recvline, stdout);
        write(sockfd, line, n );
    }
}



int
main(int argc, char **argv)
{
	int sockfd,sock,connfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in servaddr;

	if (argc != 3)
		DieWithError( "usage: tcp-client <Server-IPaddress> <Server-Port>" );
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	sendMessage(stdin, sockfd);







    echoServPort = atoi(argv[1]);  

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        DieWithError("server: socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));   
    echoServAddr.sin_family = AF_INET;                
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(echoServPort);      

    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("server: bind() failed");
  
	if (listen(sock, BACKLOG) < 0 )
		DieWithError("server: listen() failed");

	cliAddrLen = sizeof(echoClntAddr);
	connfd = accept( sock, (struct sockaddr *) &echoClntAddr, &cliAddrLen );

	EchoString(connfd);
	close(connfd);
	exit(0);
}
