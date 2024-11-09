#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <fcntl.h>  // For fcntl
#include <errno.h>  // For errno
#include "helper.h"


void *handle_storage_server(void *arg) {
    StorageServerConnectionInfo *ssc_info = (StorageServerConnectionInfo *) arg;
    int storage_socket = ssc_info->socket;
    StorageServerInfo ss_info;



    // Receive initial registration data
    if (recv_good(storage_socket, &ss_info, sizeof(ss_info)) <= 0) {
        perror("Failed to receive storage server info or client disconnected");
        close(storage_socket);
        free(ssc_info);
        return NULL;
    }
    ss_info.clientInfo = *ssc_info;


    printf("Storage Server registered:\n");
    printf("IP: %s\n", inet_ntoa(ssc_info->addr.sin_addr));
    printf("NM Port: %d, Client Port: %d\n", ntohs(ssc_info->addr.sin_port), ss_info.portClient);

    // Send "like" message every 5 seconds
    while (1) {
        const char *like_message = "like";
        if (send_good(storage_socket, like_message, strlen(like_message)) < 0) {
            perror("Failed to send 'like' message");
            break;
        }
        printf("Sent 'like' to Storage Server at %s:%d\n", inet_ntoa(ssc_info->addr.sin_addr),
               ntohs(ssc_info->addr.sin_port));
        sleep(3);

    }

    close(storage_socket);
    free(ssc_info);
    return NULL;
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, storage_addr;
    socklen_t addr_size = sizeof(storage_addr);

    // Create the server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    if (setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) < 0) {
        // So that the socket does not get buffered causing a lot of problems
        printf("setsockopt TCP_NODELAY failed");
        close(server_socket);
        return 1;
    }


    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind and listen
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Naming Server is running on port %d...\n", SERVER_PORT);

    // Accept connections
    while (1) {
        int storage_socket = accept(server_socket, (struct sockaddr *) &storage_addr, &addr_size);
        if (storage_socket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No connections yet, continue to the next iteration
                continue;
            } else {
                perror("Accept failed");
                continue;
            }
        }

        // Allocate and initialize client info
        StorageServerConnectionInfo *ssc_info = malloc(sizeof(StorageServerConnectionInfo));
        ssc_info->socket = storage_socket;
        ssc_info->addr = storage_addr;
        printf("SS Server connected - Socket: %d, IP: %s, Port: %d\n",
               ssc_info->socket,
               inet_ntoa(ssc_info->addr.sin_addr),
               ntohs(ssc_info->addr.sin_port));

        // Create a thread to handle the Storage Server
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_storage_server, ssc_info) != 0) {
            perror("Failed to create thread");
            close(storage_socket);
            free(ssc_info);
        }
        pthread_detach(thread_id);  // Detach the thread to clean up automatically
    }

    close(server_socket);
    return 0;
}
