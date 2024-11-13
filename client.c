#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "helper.h"

typedef struct {
    int socket;
    struct sockaddr_in addr;
} StorageServerConnectionInfo;

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: %s <naming_server_ip> <naming_server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    // set address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};

    fgets(buffer, 1024, stdin);
    trim(buffer);

    if(strncmp(buffer, "READ", 4) == 0) {
        //TODO read code
    } else if(strncmp(buffer, "CREATE", 6) == 0) {
        //TODO write code
    } else if(strncmp(buffer, "STREAM", 6) == 0) {
        //TODO stream code
    } else if(strncmp(buffer, "DELETE", 6) == 0) {
        //TODO delete code
    } else if(strncmp(buffer, "WRITE", 6) == 0) {
        //TODO write code
    } else if(strncmp(buffer, "", 6) == 0) {
        //TODO stream code
    } else {
        printf("Invalid command\n");
    }    

    return 0;
}