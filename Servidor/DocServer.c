#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#define SIZE 1024

void write_file(char *buffer) {
    FILE *fp;
    char *filename = "../recv.txt";
    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error in creating file!\n");
        exit(1);
    }
    fprintf(fp, "%s", buffer);
}

int count_consonants(char *buffer) {
    int consonants = 0;
    for (int i = 0; i < SIZE; ++i) {
        //printf("Buffer: %c\n", buffer[i]);
        if ((buffer[i] >= 65 && buffer[i] <= 90) || (buffer[i] >= 97 && buffer[i] <= 122)) {
            if (buffer[i] == 'a'|| buffer[i] =='e'|| buffer[i] =='i'|| buffer[i] == 'o' || buffer[i] == 'u' || buffer[i] == 'A' || buffer[i] == 'E' || buffer[i] == 'I' || buffer[i] == 'O' || buffer[i] == 'U') {
                consonants += 0;
            } else {
                consonants += 1;
            }
        } else {
            consonants += 0;
        }
    }
    return consonants;
}

int main(int argc, char const* argv[])
{

    // create server socket similar to what was done in
    // client program
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    // string store data to send to client
    char serMsg[SIZE];

    // define server address
    struct sockaddr_in servAddr, newAddr;

    socklen_t addrSize;

    if(servSockD < 0) {
        perror("Error in sockets!\n");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = 8080;
    servAddr.sin_addr.s_addr = INADDR_ANY;

    printf("Server socket created!\n");

    // bind socket to the specified IP and port
    int e = bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

    if(e < 0) {
        perror("Error in binding!\n");
        exit(1);
    }

    printf("Binding successful!\n");

    // listen for connections
    e = listen(servSockD, 10);

    if (e == 0) {
        printf("Listening...\n");
    } else {
        perror("Error in listening!\n");
        exit(1);
    }

    // integer to hold client socket.
    addrSize = sizeof(newAddr);
    int clientSocket = accept(servSockD, (struct sockaddr*)&newAddr, &addrSize);

    // send messages to client socket
    int n = recv(clientSocket, serMsg, SIZE, 0);

    char *buffer = serMsg;

    printf("Buffer: %s\n", buffer);

    write_file(buffer);

    int consonants = count_consonants(buffer);

    printf("Consonants: %d\n", consonants);

    int send_int = htonl(consonants);

    printf("Message: %d\n", send_int);

    send(clientSocket, &consonants, sizeof(serMsg), 0);

    bzero(buffer, SIZE);

    return 0;
}

