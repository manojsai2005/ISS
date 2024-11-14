


#include <netinet/in.h>
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
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


// https://chatgpt.com/share/6734fa7e-8ce8-8011-9a03-3cae826783eb for the file/folder handling
// Helper function to parse command into arguments
void parse_command(char *command, char **args) {
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
}

// Create a directory
int create_directory(char *command) {
    char *args[2];
    parse_command(command, args);

    if (args[1] == NULL) {
        fprintf(stderr, "Invalid command format\n");
        return -1;
    }

    

    if (mkdir(args[1], 0777) == -1) {
        printf("Could not create folder @ %s\n",args[1]);
        perror("mkdir");
        return -1;
    }
    printf("Created folder @ %s\n",args[1]);
    return 0;
}

// Delete a directory
int delete_directory(char *command) {
    char *args[2];
    parse_command(command, args);

    if (args[1] == NULL) {
        fprintf(stderr, "Invalid command format\n");
        return -1;
    }



    if (rmdir(args[1]) == -1) {
        printf("Could not delete folder @ %s\n",args[1]);
        perror("rmdir");
        return -1;
    }
    printf("Deleted folder @ %s\n",args[1]);
    return 0;
}

// Create a file
int create_file(char *command) {
    char *args[2];
    parse_command(command, args);

    if (args[1] == NULL) {
        fprintf(stderr, "Invalid command format\n");
        return -1;
    }

    FILE *file = fopen(args[1], "w");
    if (file == NULL) {
        printf("Could not create file @ %s\n",args[1]);
        perror("fopen");
        return -1;
    }
    printf("Created File @ %s\n",args[1]);

    fclose(file);
    return 0;
}

// List files in a directory
int list_file(char *command) {
    char *args[2];
    parse_command(command, args);

    if (args[1] == NULL) {
        fprintf(stderr, "Invalid command format\n");
        return -1;
    }

    DIR *dir = opendir(args[1]);
    if (dir == NULL) {
        printf("Could not list folder @ %s\n",args[1]);
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}

void trim(char *str) {
    char *start = str;
    char *end;
    while (*start && isspace((unsigned char)*start)) start++;
    if (*start == 0) {
        *str = '\0';
        return;
    }
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}