#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080 // Choose a port number

int main() {
    int server_socket;
    struct sockaddr_in server_address;
    socklen_t addr_size;
    int client_socket;
    char buffer[1024];

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept a connection
    addr_size = sizeof(server_address);
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&server_address, &addr_size);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Receive data from client
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Received from client: %s\n", buffer);

        // Send response back to client
        char response[] = "Hello from server!";
        send(client_socket, response, strlen(response), 0);

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
