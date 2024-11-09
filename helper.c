


#include <netinet/in.h>
#include "helper.h"
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



// Modified recv function to handle errors and disconnects
ssize_t recv_good(int sockfd, void *buf, size_t len) {
    ssize_t bytes_received = recv(sockfd, buf, len, 0);
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0; // No data available, just return
        } else {
            perror("recv failed");
            return -1; // An error occurred
        }
    }
    return bytes_received;
}

// Modified send function to handle errors
ssize_t send_good(int sockfd, const void *buf, size_t len) {
    ssize_t bytes_sent = send(sockfd, buf, len, 0);
    if (bytes_sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0; // Socket is not ready to send data, just return
        } else {
            perror("send failed");
            return -1; // An error occurred
        }
    }
    return bytes_sent;
}



