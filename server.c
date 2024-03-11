#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg); // perror interpretes the error number and outputs error disp
    exit(1);
}

int main(int argc, char *argv[])
{ // argc is total number of param we pass. here 2- 1. filename 2. port number
    if (argc < 2)
    {
        fprintf(stderr, "port number not provided\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr; // sockaddr_in gives internet address. included in netinet
    socklen_t clilen;                       // socklen_t 32 bit datatype

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening Socket\n");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr)); // bzero clears any data it refers to
    portno = atoi(argv[1]);                       // atoi converts string into int. agrv contains the original command line arguments.argv[0]=has file name. argv[1] has port number.

    serv_addr.sin_family = AF_INET; // TO GET DATA FOR SERV_ADDR
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // host to network short

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding failed");
    }
    listen(sockfd, 5); // 5 is max limit of clients that can connect to server at time

    clilen = sizeof(cli_addr);

    // now to accept the connection. new file discriptor recieved whenever new connection is accepted

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("Error on Accept");
    }

    // now code for communication btw client and server
    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255); // must be a correspondingwrite in client code
        if (n < 0)
            error("Error on reading");
        printf("Client : %s  \n", buffer);

        // now for server to reply
        bzero(buffer, 255);
        fgets(buffer, 255, stdin); // function reads bytes from stream. here the input stream into the array pointed to

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            error("Error on writing");
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}