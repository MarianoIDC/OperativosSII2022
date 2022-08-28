#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket APIs
#include <string.h>
#include <arpa/inet.h> 
//#define SIZE 8192

/*void send_file(FILE *file, int socket) {
    char data[SIZE];
    while (fgets(data, SIZE, file) != NULL) {
        if (send(socket, data, sizeof(data), 0) == -1) {
            perror("Error in sending data!\n");
            exit(1);
        }
    }
    // closing the file
    bzero(data, SIZE);
    printf("Archivo enviado\n");
    fclose(file);
}*/

void DocClient(char* ip, int port) {

    printf("############################\n");

    struct sockaddr_in servAddr;
    FILE *fp;
    
    do{
        int sockD = socket(AF_INET, SOCK_STREAM, 0);
        
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
        
        char filename[20];
        char end[] = "end";

        printf("Ingrese la ruta del archivo:\n");
        scanf("%s", filename);

        printf("Filename: %s\n", filename);

        int comparacion = strcmp(filename, &end);
        
        if(comparacion==0){
            close(sockD);
            exit(1);
        } else {
            fp = fopen(filename, "r");

            if (fp == NULL) {
                perror("Error opening file!\n");
            } else {
                fseek(fp, 0L, SEEK_END);
  
                // calculating the size of the file
                int file_size = ftell(fp);

                file_size = file_size*2;

                fseek(fp, 0L, 0);

                send(sockD, &file_size, sizeof(file_size), 0);

                char data[file_size];

                if (fgets(data, file_size, fp) != NULL)
                {
                    puts(data);
                }
                
                send(sockD, data, sizeof(data), 0);

                printf("Enviado!\n");

                int received_int = 0;

                recv(sockD, &received_int, sizeof(received_int), 0);

                printf("Message: %d\n", received_int);
            }
            printf("############################\n");
        }
    }while(1);
    
}

int main(int argc, char const* argv[]) {

    if (argc == 3){
        char *ip = argv[1];
        char *port = argv[2];
        int PORT = atoi(port);
        DocClient(ip, PORT);
    }
    else
    {
        printf("[-]Invalid number of argument, usage is %s [IPSERVER] [PORT]\n",argv[0]);
    }

    return 0;
}

