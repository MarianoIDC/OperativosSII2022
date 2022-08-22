#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket APIs
#include <string.h>
#define SIZE 1024

void send_file(FILE *file, int socket) {
    char data[SIZE] = {0};
    while (fgets(data, SIZE, file) != NULL) {
        if (send(socket, data, sizeof(data), 0) == -1) {
            perror("Error in sending data!\n");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main() {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    FILE *fp;
    char *filename = "../send.txt";

    if(sockD < 0) {
        perror("Error in sockets!\n");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = 8080;
    servAddr.sin_addr.s_addr = INADDR_ANY;

    printf("Client socket created!\n");

    int e = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

    if(e == -1) {
        perror("Error in connection!\n");
        exit(1);
    }

    printf("Connection successful!\n");

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file!\n");
        exit(1);
    }

    send_file(fp, sockD);

    printf("Successful!\n");

    int received_int = 0;

    recv(sockD, &received_int, sizeof(received_int), 0);

    printf("Message: %d\n", received_int);

    close(sockD);

    return 0;
}

