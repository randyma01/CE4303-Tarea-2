#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // Define the port on which the server will listen for incoming connections
    int port = 1717; // Change 1717 to the port you want to use

    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address to bind the socket to
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    // Accept incoming connections
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Loop to receive and respond to messages from the client
    while (1)
    {
        char buffer[1024];
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1)
        {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        if (bytes_received == 0)
        {
            // Client has disconnected
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received message from client: %s\n", buffer);

        // Send a response back to the client
        char response[2048]; // Increased buffer size
        snprintf(response, sizeof(response), "Server received: %s", buffer);
        if (send(client_socket, response, strlen(response), 0) == -1)
        {
            perror("Error sending response");
            exit(EXIT_FAILURE);
        }

        printf("Response sent to client: %s\n", response);
    }

    // Close the client socket
    close(client_socket);

    // Close the server socket
    close(server_socket);

    return 0;
}
