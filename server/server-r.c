#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

// Function to receive a file from the client and save it
void receiveFile(int socket)
{
    char buffer[8192]; // A reasonably sized buffer for file transfer
    ssize_t bytesRead;
    char filePath[1024];

    // Receive the file name from the client
    bytesRead = recv(socket, filePath, sizeof(filePath), 0);
    if (bytesRead == -1)
    {
        perror("Error receiving file name");
        exit(EXIT_FAILURE);
    }

    filePath[bytesRead] = '\0';

    // Create and open the file
    FILE *file = fopen(filePath, "wb");
    if (!file)
    {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    // Receive and write the file data
    while ((bytesRead = recv(socket, buffer, sizeof(buffer), 0)) > 0)
    {
        fwrite(buffer, 1, bytesRead, file);
    }

    if (bytesRead == -1)
    {
        perror("Error receiving file data");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Close the file
    fclose(file);
}

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

    while (1)
    {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        // Receive and save the file from the client
        receiveFile(client_socket);
        printf("File received and saved\n");

        // Send a response back to the client
        char response[] = "File received successfully";
        if (send(client_socket, response, strlen(response), 0) == -1)
        {
            perror("Error sending response");
            exit(EXIT_FAILURE);
        }

        printf("Response sent to client\n");

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (never reached in this example)
    close(server_socket);

    return 0;
}
