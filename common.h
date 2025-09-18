#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

// Protocol message types
typedef enum {
    MSG_HELLO = 1,
    MSG_ECHO = 2,
    MSG_CALC = 3,
    MSG_TIME = 4,
    MSG_BYE = 5,
    MSG_RESPONSE = 100,
    MSG_ERROR = 999
} message_type_t;

// Protocol message structure
typedef struct {
    message_type_t type;
    int length;
    char data[BUFFER_SIZE - sizeof(message_type_t) - sizeof(int)];
} message_t;

// Function prototypes
void print_usage(const char* program_name);
int send_message(int socket, message_t* msg);
int receive_message(int socket, message_t* msg);
void print_message(const message_t* msg);

#endif // COMMON_H