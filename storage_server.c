// StorageServer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "helper.h"

// Function to connect to the Naming Server and listen for messages
void connect_to_nm(const char *nm_ip, int nm_port,StorageServerInfo* ssi) {
    int sock;
    struct sockaddr_in nm_addr;
    char buffer[BUFFER_SIZE];

    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) < 0) {
        // So that the socket does not get buffered causing a lot of problems
        printf("setsockopt TCP_NODELAY failed");
        close(sock);
        return;
    }
    // Set up Naming Server address
    nm_addr.sin_family = AF_INET;
    nm_addr.sin_port = htons(nm_port);
    if (inet_pton(AF_INET, nm_ip, &nm_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to Naming Server
    if (connect(sock, (struct sockaddr *) &nm_addr, sizeof(nm_addr)) < 0) {
        perror("Connection to Naming Server failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Connected to Naming Server at %s:%d\n", nm_ip, nm_port);

    if (send_good(sock, (void *)ssi, sizeof(StorageServerInfo)) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }




    // Listen for "like" messages
    while (1) {
        int bytes_received = recv_good(sock, buffer, BUFFER_SIZE);
        if (bytes_received <= 0) {
            printf("Connection closed by Naming Server\n");
            break;
        }
        printf("Received from Naming Server: %s\n", buffer);
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <NM_IP> <NM_Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command-line arguments
    const char *nm_ip = argv[1];
    int nm_port = atoi(argv[2]);

    StorageServerInfo ssi;
    ssi.portClient=CLIENT_PORT;


    // Connect to the Naming Server and listen for messages
    connect_to_nm(nm_ip, nm_port,&ssi);

    return 0;
}
