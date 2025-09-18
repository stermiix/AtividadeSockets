#include "common.h"
#include <time.h>
#include <math.h>

void handle_client(int client_socket);
void process_message(message_t* request, message_t* response);

int main(int argc, char* argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int port = DEFAULT_PORT;
    
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number. Using default port %d\n", DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    }
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }
    
    // Allow socket reuse
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(server_socket);
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error listening");
        close(server_socket);
        exit(1);
    }
    
    printf("Server listening on port %d...\n", port);
    
    // Accept and handle clients
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Error accepting client");
            continue;
        }
        
        printf("Client connected from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));
        
        handle_client(client_socket);
        
        printf("Client disconnected\n");
        close(client_socket);
    }
    
    close(server_socket);
    return 0;
}

void handle_client(int client_socket) {
    message_t request, response;
    
    while (1) {
        if (receive_message(client_socket, &request) <= 0) {
            break;
        }
        
        printf("Received message: ");
        print_message(&request);
        
        process_message(&request, &response);
        
        if (send_message(client_socket, &response) <= 0) {
            break;
        }
        
        if (request.type == MSG_BYE) {
            break;
        }
    }
}

void process_message(message_t* request, message_t* response) {
    memset(response, 0, sizeof(message_t));
    response->type = MSG_RESPONSE;
    
    switch (request->type) {
        case MSG_HELLO:
            strcpy(response->data, "Hello! Welcome to the socket server!");
            response->length = strlen(response->data);
            break;
            
        case MSG_ECHO:
            snprintf(response->data, sizeof(response->data), "Echo: %s", request->data);
            response->length = strlen(response->data);
            break;
            
        case MSG_CALC: {
            double a, b;
            char op;
            if (sscanf(request->data, "%lf %c %lf", &a, &op, &b) == 3) {
                double result = 0;
                int valid = 1;
                
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': 
                        if (b != 0) result = a / b;
                        else valid = 0;
                        break;
                    default: valid = 0; break;
                }
                
                if (valid) {
                    snprintf(response->data, sizeof(response->data), "Result: %.2f", result);
                } else {
                    strcpy(response->data, "Error: Invalid operation or division by zero");
                }
            } else {
                strcpy(response->data, "Error: Invalid calculation format. Use: number operator number");
            }
            response->length = strlen(response->data);
            break;
        }
        
        case MSG_TIME: {
            time_t now = time(NULL);
            char* time_str = ctime(&now);
            time_str[strlen(time_str) - 1] = '\0'; // Remove newline
            snprintf(response->data, sizeof(response->data), "Server time: %s", time_str);
            response->length = strlen(response->data);
            break;
        }
        
        case MSG_BYE:
            strcpy(response->data, "Goodbye! Connection will be closed.");
            response->length = strlen(response->data);
            break;
            
        default:
            response->type = MSG_ERROR;
            strcpy(response->data, "Error: Unknown message type");
            response->length = strlen(response->data);
            break;
    }
}

