#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //Librería que crea los sockets
#include <string.h>
#include <arpa/inet.h> //Librería para las conexiones

void DocClient(char* ip, int port) {

    printf("############################\n");

    //Define la dirección del cliente
    struct sockaddr_in servAddr;
    //Define el puntero al archivo
    FILE *fp;
    
    do{
        //Crea el socket del cliente
        int sockD = socket(AF_INET, SOCK_STREAM, 0);
        
        if(sockD < 0) {
            perror("Error in sockets!\n");
            exit(1);
        }

        //Define los parámetros del socket
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = port;
        servAddr.sin_addr.s_addr = inet_addr(ip);

        printf("Client socket created!\n");

        //Conecta el socket con sus parámetros
        int e = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

        if(e == -1) {
            perror("Error in connection!\n");
            exit(1);
        }

        printf("Connection successful!\n");
        
        //Recibe la ruta del archivo del usuario
        char filename[20];
        char end[] = "end";

        printf("Ingrese la ruta del archivo:\n");
        scanf("%s", filename);

        //Compara la ruta del archivo con el string end, si es igual entonces cierra el socket
        int comparacion = strcmp(filename, &end);
        
        if(comparacion == 0){
            close(sockD);
            exit(1);
        } else {
            //Abre el archivo de prueba
            fp = fopen(filename, "r");

            if (fp == NULL) {
                perror("Error opening file!\n");
            } else {
                fseek(fp, 0L, SEEK_END);
  
                //Calcula el tamaño del archivo
                int file_size = ftell(fp);

                file_size = file_size*2;

                fseek(fp, 0L, 0);

                //Envía el tamaño del archivo al servidor para ser aprovechado
                send(sockD, &file_size, sizeof(file_size), 0);

                //Buffer que recibe los datos del archivo
                char data[file_size];

                if (fgets(data, file_size, fp) != NULL)
                {
                    puts(data);
                }
                
                //Envía el buffer con los contenidos del archivo al servidor
                send(sockD, data, sizeof(data), 0);

                printf("Enviado!\n");

                //Cantidad de consonantes enviadas por el cliente
                int received_int = 0;

                //Recibe la cantidad de consonantes del cliente
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

