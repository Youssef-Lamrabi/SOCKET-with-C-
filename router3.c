#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "youssef.h"
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT_ROUTER3 9093  // Assurez-vous que ce port est correct
#define PORT_SERVER 9094   // Assurez-vous que ce port correspond à celui du serveur
#define SA struct sockaddr

void funcServer(int connfd, char *trame) {
    char buff[MAX];
    ssize_t n = read(connfd, buff, sizeof(buff));
    if (n < 0) {
        perror("read from previous router socket failed");
        return;
    }
    printf("\n--TRAME FROM PREVIOUS ROUTER: %s ", buff);
    if (CrcRecieve(buff)) {
        printf("\n--TRAME Successfully received");
    } else {
        printf("\n--TRAME Failed in receiving");
    }
    strcpy(trame, buff);
    bzero(buff, MAX);
}

void funcClient(int sockfd, char *buff) {
    printf("TRAME FROM ROUTER 3: %s ", buff);
    ssize_t n = write(sockfd, buff, 65);
    if (n < 0) {
        perror("write to server socket failed");
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT_ROUTER3);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        perror("socket bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully binded..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Router 3 listening..\n");
    }
    len = sizeof(cli);

    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("server accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Router 3 accepted the client...\n");
    }

    char trame[65];
    funcServer(connfd, trame);
    close(connfd);

    int sockfd2;
    struct sockaddr_in servaddr2;

    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr2, sizeof(servaddr2));

    servaddr2.sin_family = AF_INET;
    servaddr2.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr2.sin_port = htons(PORT_SERVER);

    if (connect(sockfd2, (SA*)&servaddr2, sizeof(servaddr2)) != 0) {
        perror("connection with the server failed");
        close(sockfd2);
        exit(EXIT_FAILURE);
    } else {
        printf("Connected to the server..\n");
    }

    funcClient(sockfd2, trame);
    close(sockfd2);
    return 0;
}
