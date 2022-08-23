#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket APIs
#include <string.h>
#include <arpa/inet.h> 
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

    //Parámetros a pedirle al usuario
    char *filename = "./send.txt";
    char *ip = "172.18.0.1";
    int port = 8080;

    //Se pide al usuario los parámetros del cliente
/*  printf("Enter the file: ");
    scanf("\n%c", filename);

    printf("Enter the IP: \n");
    scanf("%c", ip);

    printf("Enter the port: \n");
    scanf("%d", &port);*/

    if(sockD < 0) {
        perror("Error in sockets!\n");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = port;
    servAddr.sin_addr.s_addr = inet_addr(ip);

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

