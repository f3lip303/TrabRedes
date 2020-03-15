// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

// Driver code
int main()
{
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in servaddr;
    char *line = NULL;
    ssize_t numBytesSent, numBytesRcvd;
    ssize_t read = 0;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    /*sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *)&servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
*/
    //while (1)
    //{
        fputs("Client: ", stdout);
        //read = getline(&line, &len, stdin);
        printf("SYN\n");
        numBytesSent = sendto(sockfd, "SYN", strlen(hello),
                              MSG_CONFIRM, (const struct sockaddr *)&servaddr,
                              sizeof(servaddr));

        if (numBytesSent < 0)
        {
            perror("send() falhou\n");
            exit(EXIT_FAILURE);
        }

        numBytesRcvd = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                MSG_WAITALL, (struct sockaddr *)&servaddr,
                                &len);
        if (numBytesRcvd < 0)
        {
            perror("recv() falhou\n");
            exit(EXIT_FAILURE);
        }

        fputs("Server: ", stdout);
        fputs(buffer, stdout);
        fputc('\n', stdout);
   // }

    close(sockfd);
    return 0;
}
