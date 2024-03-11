/*
how to pass command line arguments

filename server_ipaddress portnumber(same as server)

argv[0] filename
agrv[1] server_ipaddress
agrv[2] portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg); // perror interpretes the error number and outputs error disp
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; // included in netdb. stores info abt server

    char buffer[255];
    if (argv < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Eroor opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); // copies n bytes from *server to serv_addr
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection failed");
    }

    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            error("Error in writing");
        }

        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0)
        {
            error("Error in reading");
        }

        printf("Server: %s", buffer);
        // code to get out of the loop
        int i = strncmp("Bye", buffer, 3); // 3 iswhenever the buffer gets the data "bye" from the serverwhich is of lenght 3(cuz we need to specify the string len)
        if (i == 0)
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}