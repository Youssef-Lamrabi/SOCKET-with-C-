#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "youssef.h"

#define MAX 65
#define PORT 9094  // Changez ce port
#define SA struct sockaddr

void func(int connfd) {
    char buff[MAX];
    ssize_t n = read(connfd, buff, sizeof(buff));
    if (n < 0) {
        perror("read from client socket failed");
        return;
    }
    printf("\n--TRAME FROM ROUTER: %s\n", buff);
    if (CrcRecieve(buff)) {
        printf("\n--TRAME Successfully received");
    } else {
        printf("\n--TRAME Failed in receiving");
    }
    bzero(buff, MAX);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assignation de l'IP et du port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Liaison du socket à l'adresse IP et au port
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        perror("socket bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully binded..\n");
    }

    // Écoute des connexions entrantes
    if ((listen(sockfd, 5)) != 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Server listening on port %d..\n", PORT);
    }
    len = sizeof(cli);

    // Acceptation des connexions client
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("server accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Server accepted the client...\n");
    }

    // Traitement de la connexion client
    func(connfd);
    close(connfd);
    close(sockfd);
    return 0;
}
