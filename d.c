// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8081
#define MAXLINE 1024

// Driver code
int main()
{

    int sockfd;
    char buffer[MAXLINE];
    char *testando = "testando";
    struct sockaddr_in servaddr;
    char *line = NULL;
    ssize_t numBytesSent, numBytesRcvd;
    ssize_t read = 0;

    struct sockaddr_in cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int n, len;

    //while (1)
    //{

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
    fputs("Client: ", stdout);

    read = getline(&line, &len, stdin);
    //line = "SYNACK";
    numBytesSent = sendto(sockfd, line, strlen(testando),
                          MSG_CONFIRM, (const struct sockaddr *)&servaddr,
                          sizeof(servaddr));

    if (numBytesSent < 0)
    {
        perror("send() falhou\n");
        exit(EXIT_FAILURE);
    }
    //}

    close(sockfd);
    return 0;
}
