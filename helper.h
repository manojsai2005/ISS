//
// Created by George Rahul on 09/11/24.
//

#ifndef NFS_HELPER_H
#define NFS_HELPER_H
#define SERVER_PORT 9001
#define BUFFER_SIZE 1024
#define CLIENT_PORT 9024
typedef struct {
    int socket;
    struct sockaddr_in addr;
} StorageServerConnectionInfo;

typedef struct {
    StorageServerConnectionInfo clientInfo;
    int portClient;
    int socket;

} StorageServerInfo;


ssize_t recv_good(int sockfd, void *buf, size_t len);
ssize_t send_good(int sockfd, const void *buf, size_t len);
int list_file(char *command);
int create_file(char *command);
int delete_directory(char *command);
int create_directory(char *command);
void trim(char *str);

#endif //NFS_HELPER_H
