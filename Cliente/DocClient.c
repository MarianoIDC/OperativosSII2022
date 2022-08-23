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

int DocClient(char* ip, int port, const char* filename) {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    FILE *fp;

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
}

int main(int argc, char const* argv[]) {
    
    if (argc == 4)
    {
        char *ip = argv[1];
        char *port = argv[2];
        const char* filename = argv[3];
        //char *format = argv[3];
        int PORT = atoi(port);
        //int extension=detect_format(format);
        //printf("Extension %d",extension);
        //return client(filename, pixels, extension);
        return DocClient(ip, PORT, filename);

    }
    else
    {
        printf("[-]Invalid number of argument, usage is %s [IPSERVER] [PORT] [FILENAME] \n",argv[0]);
    }

    return 0;
}

