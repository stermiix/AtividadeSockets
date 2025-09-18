#include "common.h"

int send_message(int socket, message_t* msg) {
    int total_size = sizeof(message_type_t) + sizeof(int) + msg->length;
    return send(socket, msg, total_size, 0);
}

int receive_message(int socket, message_t* msg) {
    // First receive the header (type and length)
    int header_size = sizeof(message_type_t) + sizeof(int);
    int bytes_received = recv(socket, msg, header_size, MSG_WAITALL);
    
    if (bytes_received <= 0) {
        return bytes_received;
    }
    
    // Then receive the data if length > 0
    if (msg->length > 0) {
        int data_received = recv(socket, msg->data, msg->length, MSG_WAITALL);
        if (data_received <= 0) {
            return data_received;
        }
        bytes_received += data_received;
    }
    
    msg->data[msg->length] = '\0'; // Null terminate
    return bytes_received;
}

void print_message(const message_t* msg) {
    const char* type_str;
    switch (msg->type) {
        case MSG_HELLO: type_str = "HELLO"; break;
        case MSG_ECHO: type_str = "ECHO"; break;
        case MSG_CALC: type_str = "CALC"; break;
        case MSG_TIME: type_str = "TIME"; break;
        case MSG_BYE: type_str = "BYE"; break;
        case MSG_RESPONSE: type_str = "RESPONSE"; break;
        case MSG_ERROR: type_str = "ERROR"; break;
        default: type_str = "UNKNOWN"; break;
    }
    printf("Type: %s, Length: %d, Data: %s\n", type_str, msg->length, msg->data);
}

void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help           Show this help message\n");
    printf("  -i, --interactive    Run in interactive mode\n");
    printf("  -p <port>            Server port (default: %d)\n", DEFAULT_PORT);
    printf("  -s <server_ip>       Server IP address (default: 127.0.0.1)\n");
    printf("\nExamples:\n");
    printf("  %s                   # Run test mode with default settings\n", program_name);
    printf("  %s -i                # Run in interactive mode\n", program_name);
    printf("  %s -p 9000 -s 192.168.1.100  # Connect to specific server\n", program_name);
}