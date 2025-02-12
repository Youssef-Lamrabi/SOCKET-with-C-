#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "youssef.h"

#define MAX 80 // Taille maximale du buffer
#define PORT 9090 // Port du routeur
#define SA struct sockaddr

void func(int sockfd, char *buff) {
    printf("TRAME FROM CLIENT: %s\n", buff);
    ssize_t n = write(sockfd, buff, 65); // Envoie du message au routeur
    if (n < 0) {
        perror("write to server socket failed");
    }
}

int main() {
    char message[65]; // Trame de 64 caractères + '\0'
    char trame[65] = "0000000000000000000000000000000000000000111101110000000000000000";
    CrcTrasnfert(trame, message);
    printf("Final message to be sent: %s\n", trame);

    int sockfd;
    struct sockaddr_in servaddr;

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assignation de l'IP et du port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connexion du socket client au socket du routeur
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connection with the router failed");
        close(sockfd);
        exit(0);
    } else {
        printf("Connected to the router..\n");
    }

    // Envoi de la trame
    func(sockfd, trame);
    close(sockfd);
    return 0;
}
