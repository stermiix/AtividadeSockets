#include "common.h"

void interactive_mode(int socket);
void send_test_messages(int socket);

int main(int argc, char* argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;
    char* server_ip = "127.0.0.1";
    int port = DEFAULT_PORT;
    int interactive = 0;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
            interactive = 1;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            server_ip = argv[++i];
        }
    }
    
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number\n");
        return 1;
    }
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server IP address\n");
        close(client_socket);
        exit(1);
    }
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        close(client_socket);
        exit(1);
    }
    
    printf("Connected to server %s:%d\n", server_ip, port);
    
    if (interactive) {
        interactive_mode(client_socket);
    } else {
        send_test_messages(client_socket);
    }
    
    close(client_socket);
    return 0;
}

void interactive_mode(int socket) {
    message_t request, response;
    char input[BUFFER_SIZE];
    char command[32], data[BUFFER_SIZE];
    
    printf("\nInteractive mode - Available commands:\n");
    printf("  hello                - Send hello message\n");
    printf("  echo <message>       - Echo a message\n");
    printf("  calc <a> <op> <b>    - Calculate (ops: +, -, *, /)\n");
    printf("  time                 - Get server time\n");
    printf("  bye                  - Disconnect\n");
    printf("  help                 - Show this help\n\n");
    
    while (1) {
        printf("client> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) {
            continue;
        }
        
        // Parse command
        memset(&request, 0, sizeof(request));
        
        if (sscanf(input, "%31s %[^\n]", command, data) < 1) {
            continue;
        }
        
        if (strcmp(command, "help") == 0) {
            printf("Available commands:\n");
            printf("  hello                - Send hello message\n");
            printf("  echo <message>       - Echo a message\n");
            printf("  calc <a> <op> <b>    - Calculate (ops: +, -, *, /)\n");
            printf("  time                 - Get server time\n");
            printf("  bye                  - Disconnect\n");
            continue;
        } else if (strcmp(command, "hello") == 0) {
            request.type = MSG_HELLO;
            strcpy(request.data, "");
            request.length = 0;
        } else if (strcmp(command, "echo") == 0) {
            request.type = MSG_ECHO;
            strcpy(request.data, data);
            request.length = strlen(request.data);
        } else if (strcmp(command, "calc") == 0) {
            request.type = MSG_CALC;
            strcpy(request.data, data);
            request.length = strlen(request.data);
        } else if (strcmp(command, "time") == 0) {
            request.type = MSG_TIME;
            strcpy(request.data, "");
            request.length = 0;
        } else if (strcmp(command, "bye") == 0) {
            request.type = MSG_BYE;
            strcpy(request.data, "");
            request.length = 0;
        } else {
            printf("Unknown command: %s. Type 'help' for available commands.\n", command);
            continue;
        }
        
        // Send request
        if (send_message(socket, &request) <= 0) {
            printf("Error sending message\n");
            break;
        }
        
        // Receive response
        if (receive_message(socket, &response) <= 0) {
            printf("Error receiving response\n");
            break;
        }
        
        printf("Server response: %s\n", response.data);
        
        if (request.type == MSG_BYE) {
            break;
        }
    }
}

void send_test_messages(int socket) {
    message_t request, response;
    
    printf("\nSending test messages to server:\n\n");
    
    // Test 1: Hello
    printf("Test 1: Hello message\n");
    memset(&request, 0, sizeof(request));
    request.type = MSG_HELLO;
    request.length = 0;
    
    if (send_message(socket, &request) > 0 && receive_message(socket, &response) > 0) {
        printf("Response: %s\n\n", response.data);
    }
    
    // Test 2: Echo
    printf("Test 2: Echo message\n");
    memset(&request, 0, sizeof(request));
    request.type = MSG_ECHO;
    strcpy(request.data, "This is a test message!");
    request.length = strlen(request.data);
    
    if (send_message(socket, &request) > 0 && receive_message(socket, &response) > 0) {
        printf("Response: %s\n\n", response.data);
    }
    
    // Test 3: Calculator
    printf("Test 3: Calculator (15 + 25)\n");
    memset(&request, 0, sizeof(request));
    request.type = MSG_CALC;
    strcpy(request.data, "15 + 25");
    request.length = strlen(request.data);
    
    if (send_message(socket, &request) > 0 && receive_message(socket, &response) > 0) {
        printf("Response: %s\n\n", response.data);
    }
    
    // Test 4: Time
    printf("Test 4: Server time\n");
    memset(&request, 0, sizeof(request));
    request.type = MSG_TIME;
    request.length = 0;
    
    if (send_message(socket, &request) > 0 && receive_message(socket, &response) > 0) {
        printf("Response: %s\n\n", response.data);
    }
    
    // Test 5: Bye
    printf("Test 5: Goodbye message\n");
    memset(&request, 0, sizeof(request));
    request.type = MSG_BYE;
    request.length = 0;
    
    if (send_message(socket, &request) > 0 && receive_message(socket, &response) > 0) {
        printf("Response: %s\n\n", response.data);
    }
}

