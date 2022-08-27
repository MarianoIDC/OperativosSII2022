#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <arpa/inet.h> 
#define SIZE 1024

void write_log(char *log){
    
    FILE *log_file; 
    log_file = fopen("./log.txt", "w");
 
    if (NULL == log_file) {
        printf("file can't be opened \n");
    }
    printf("Escribiendo en el Log\n");
    printf("%s\n", log);
    fprintf(log_file, ">>>>>> %s\n", log); // write to file

    printf("Se supone que ya se escribio\n");
}

void write_file(char *buffer) {
    FILE *fp;
    char *filename = "./recv.txt";
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
        char *error = "Error in sockets!\n";
        perror(error);
        // write_file(error);
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = 8080;
    // servAddr.sin_addr.s_addr = inet_addr("192.168.122.180");
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    char *message_1 = "Server socket created!";
    printf("%s\n", message_1);
    write_log(message_1);

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

    do {

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

    } while(1);
}

