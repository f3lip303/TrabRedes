#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define PORT2 8081
#define MAXLINE 1024

//struct sockaddr_in servaddr1, servaddr2;

int meuSocket()
{
    int sockfd;
    struct sockaddr_in servaddr1, servaddr2;
    struct sockaddr_in cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr1, 0, sizeof(servaddr1));

    // Filling server information
    servaddr1.sin_family = AF_INET; // IPv4
    servaddr1.sin_addr.s_addr = INADDR_ANY;
    servaddr1.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr1,
             sizeof(servaddr1)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
int meuSocket2()
{
    int sockfd;
    struct sockaddr_in servaddr1, servaddr2;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr2, 0, sizeof(servaddr2));

    // Filling server information
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_port = htons(PORT2);
    servaddr2.sin_addr.s_addr = INADDR_ANY;

    return sockfd;
}

void myAccept(int sockfd, int sockfd2)
{
    struct sockaddr_in servaddr1, servaddr2;

    servaddr1.sin_family = AF_INET; // IPv4
    servaddr1.sin_addr.s_addr = INADDR_ANY;
    servaddr1.sin_port = htons(PORT);

    servaddr2.sin_family = AF_INET; // IPv4
    servaddr2.sin_addr.s_addr = INADDR_ANY;
    servaddr2.sin_port = htons(PORT2);
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    char *line = NULL;
    int len, n, len2;
    struct sockaddr_in cliaddr, cliaddr2;
    ssize_t read = 0;
    ssize_t numBytesSent, numBytesRcvd;

    len = sizeof(cliaddr); //len is value/resuslt
    len2 = sizeof(cliaddr2);
    /*n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&cliaddr,
                 &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
           len);
    printf("Hello message sent.\n");*/

    while (true)
    {
        //--------------------------- IDA ------------------------------------

        numBytesRcvd = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                MSG_WAITALL, (struct sockaddr *)&servaddr1,
                                &len);
        if (numBytesRcvd < 0)
        {
            perror("recvfrom() falhou");
            exit(EXIT_FAILURE);
        }
        fputs("Client: ", stdout);
        fputs(buffer, stdout);
        fputc('\n', stdout);

        fputs("Server: ", stdout);

        numBytesSent = sendto(sockfd2, buffer, strlen(hello),
                              MSG_CONFIRM, (const struct sockaddr *)&servaddr2,
                              len2);
        if (numBytesSent < 0)
        {
            perror("primeiro send() falhou\n");
            exit(EXIT_FAILURE);
        }
        printf("MANDOU O SENDTO");
        //--------------------------- VOLTA ------------------------------------
        
        numBytesRcvd = recvfrom(sockfd2, (char *)buffer, MAXLINE,
                                MSG_WAITALL, (struct sockaddr *)&servaddr2,
                                &len2);
       
        if (numBytesRcvd < 0)
        {
            perror("recvfrom() falhou");
            exit(EXIT_FAILURE);
        }
        printf("%s",buffer);
        fputs("Client: ", stdout);
        fputs(buffer, stdout);
        fputc('\n', stdout);

       fputs("Server: ", stdout);

        numBytesSent = sendto(sockfd, buffer, strlen(hello),
                              MSG_CONFIRM, (const struct sockaddr *)&servaddr1,
                              len);
        if (numBytesSent < 0)
        {
            perror("send() falhou\n");
            exit(EXIT_FAILURE);
        }

    }
}
int main(int argc, char const *argv[])
{
    int sockfd = meuSocket();
    int sockfd2 = meuSocket2();
    myAccept(sockfd, sockfd2);
    return 0;
}
