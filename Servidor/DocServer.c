#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <arpa/inet.h> 
//#define SIZE 8192

void write_log(char *log, char *filename){
    
    FILE *log_file; 
    log_file = fopen(filename, "a");
 
    if (NULL == log_file) {
        printf("file can't be opened \n");
    }
    printf("Escribiendo en el Log\n");
    printf("%s\n", log);
    fprintf(log_file, "%s\n", log); // write to file
    printf("Se supone que ya se escribio\n");
}

int count_consonants(char *buffer) {
    int consonants = 0;
    for (int i = 0; i < strlen(buffer); ++i) {
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

void DocServer() {
    // create server socket similar to what was done in
    // client program
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    int port;
    
    FILE *conf_fp;
    
    char log_filename[32];
    
    char recv_filename[32];
    
    conf_fp = fopen("./server.conf", "r");
    
    fscanf(conf_fp, "%d %s %s", &port, recv_filename, log_filename);
    
    fclose(conf_fp);

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
    servAddr.sin_port = port;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    char *message_1 = "Server socket created!";
    write_log(message_1, log_filename);
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
    int clientSocket;

    do {
	
        // integer to hold client socket.
        addrSize = sizeof(newAddr);
        clientSocket = accept(servSockD, (struct sockaddr*)&newAddr, &addrSize);
        printf("Socket aceptado\n");

        int buffer_size = 0;
        recv(clientSocket, &buffer_size, sizeof(buffer_size), 0);

        printf("Size: %d\n", buffer_size);

        
	// string store data to send to client
    	char serMsg[buffer_size];
    	
        // send messages to client socket
        recv(clientSocket, serMsg, sizeof(serMsg), 0);

        printf("Buffer: %s\n", serMsg);
        
        FILE* fp;
        fp = fopen(recv_filename, "a");
        if (fp == NULL) {
             perror("Error in creating file!\n");
             exit(1);
        }
        fprintf(fp, "%s\n", serMsg);
        
        int consonants = count_consonants(serMsg);
        printf("Consonants: %d\n", consonants);

        send(clientSocket, &consonants, sizeof(consonants), 0);

        system("strace -n -i -ttt -o systemcalls.txt ./DocServer");

        bzero(serMsg, buffer_size);


        printf("###############################\n");
        
    } while(1);
    
}

int main(int argc, char const* argv[])
{
    DocServer();
    return 0;
}

