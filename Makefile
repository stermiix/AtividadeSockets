# Makefile for AtividadeSockets - Socket Programming Activity
# Author: Socket Programming Activity
# Description: TCP client-server application with custom protocol

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

# Target executables
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.c
CLIENT_SRC = client.c
COMMON_SRC = common.c
COMMON_HDR = common.h

# Default target
all: $(SERVER) $(CLIENT)

# Build server
$(SERVER): $(SERVER_SRC) $(COMMON_SRC) $(COMMON_HDR)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC) $(COMMON_SRC) $(LDFLAGS)

# Build client
$(CLIENT): $(CLIENT_SRC) $(COMMON_SRC) $(COMMON_HDR)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC) $(COMMON_SRC) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(SERVER) $(CLIENT) *.o

# Run server with default port
run-server: $(SERVER)
	./$(SERVER)

# Run server with custom port
run-server-port: $(SERVER)
	./$(SERVER) 9000

# Run client in test mode
run-client: $(CLIENT)
	./$(CLIENT)

# Run client in interactive mode
run-client-interactive: $(CLIENT)
	./$(CLIENT) -i

# Run client connecting to custom server
run-client-custom: $(CLIENT)
	./$(CLIENT) -p 9000 -s 127.0.0.1

# Test the application (requires server to be running)
test: $(CLIENT)
	@echo "Note: Make sure server is running before running tests"
	@echo "Starting client test..."
	./$(CLIENT)

# Install targets (copy to /usr/local/bin)
install: $(SERVER) $(CLIENT)
	cp $(SERVER) /usr/local/bin/
	cp $(CLIENT) /usr/local/bin/

# Uninstall targets
uninstall:
	rm -f /usr/local/bin/$(SERVER)
	rm -f /usr/local/bin/$(CLIENT)

# Help target
help:
	@echo "Available targets:"
	@echo "  all                 - Build both server and client"
	@echo "  server              - Build server only"
	@echo "  client              - Build client only"
	@echo "  clean               - Remove build artifacts"
	@echo "  run-server          - Run server with default port (8080)"
	@echo "  run-server-port     - Run server with custom port (9000)"
	@echo "  run-client          - Run client in test mode"
	@echo "  run-client-interactive - Run client in interactive mode"
	@echo "  run-client-custom   - Run client with custom server settings"
	@echo "  test                - Run client tests (server must be running)"
	@echo "  install             - Install binaries to /usr/local/bin"
	@echo "  uninstall           - Remove installed binaries"
	@echo "  help                - Show this help message"

# Phony targets
.PHONY: all clean run-server run-server-port run-client run-client-interactive run-client-custom test install uninstall help