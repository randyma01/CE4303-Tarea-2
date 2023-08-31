#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

// Function to send a file to the server
void sendFile(FILE *file, int socket)
{
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(socket, buffer, bytesRead, 0) == -1)
        {
            perror("Error sending file");
            exit(EXIT_FAILURE);
        }
    }
}

int main()
{
    // Define the server and port to connect to
    char *server_ip = "127.0.0.1"; // Use "localhost" or "127.0.0.1" for local connection
    int port = 1717;               // Change 1717 to the port you want to use

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address to connect to
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Error setting up server address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Loop to send files to the server
    char filePath[1024];
    while (true)
    {
        printf("Enter the path of the image file to send (or type 'exit' to quit): ");
        fgets(filePath, sizeof(filePath), stdin);

        // Remove the newline character at the end of the file path
        size_t len = strlen(filePath);
        if (len > 0 && filePath[len - 1] == '\n')
        {
            filePath[len - 1] = '\0';
        }

        // Check if the user wants to exit
        if (strcasecmp(filePath, "exit") == 0)
        {
            break;
        }

        // Open the file
        FILE *file = fopen(filePath, "rb");
        if (!file)
        {
            perror("Error opening file");
            continue; // Skip to the next iteration if the file can't be opened
        }

        // Send the file name to the server
        if (send(client_socket, filePath, strlen(filePath), 0) == -1)
        {
            perror("Error sending file name");
            fclose(file);
            continue; // Skip to the next iteration
        }

        // Send the file data to the server
        sendFile(file, client_socket);
        printf("File sent: %s\n", filePath);

        // Close the file
        fclose(file);

        // Receive a response from the server
        char response[1024];
        ssize_t bytes_received = recv(client_socket, response, sizeof(response), 0);
        if (bytes_received == -1)
        {
            perror("Error receiving response");
            exit(EXIT_FAILURE);
        }

        response[bytes_received] = '\0';
        printf("Received response: %s\n", response);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
