#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

// Function to receive a file from the client and save it
bool receiveFile(int socket)
{

    printf("receiveFile SENTINEL");
    char buffer[1024];
    ssize_t bytesRead;
    char filePath[1024];

    // Receive the file name from the client
    bytesRead = recv(socket, filePath, sizeof(filePath), 0);
    if (bytesRead == -1)
    {
        perror("Error receiving file name");
        return false;
    }

    filePath[bytesRead] = '\0';

    // Check if the user wants to exit
    if (strcasecmp(filePath, "exit") == 0)
    {
        close(socket);
        return false; // Exit the function and indicate client to exit
    }

    // Create the 'imgs' folder if it doesn't exist
    struct stat st = {0};
    if (stat("imgs", &st) == -1)
    {
        // La carpeta 'imgs' no existe, la creamos
        if (mkdir("imgs", 0700) == -1)
        {
            perror("Error creating 'imgs' folder");
            return false;
        }
    }

    // Create and open the file in the "imgs" directory
    char imgPath[2048]; // Increased buffer size
    snprintf(imgPath, sizeof(imgPath), "imgs/%s", filePath);

    FILE *file = fopen(imgPath, "wb");
    if (!file)
    {
        perror("Error creating file");
        return false;
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
        return false;
    }

    // Close the file
    fclose(file);

    return true;
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
        printf("receiveFile SENTINEL - 1");

        bool success = receiveFile(client_socket);

        // Check if the received message is 'exit'
        if (!success)
        {
            break; // Exit the loop if 'exit' is received
        }

        // Send a response back to the client
        char response[] = "File received successfully. You can send another image.";
        if (send(client_socket, response, strlen(response), 0) == -1)
        {
            perror("Error sending response");
            exit(EXIT_FAILURE);
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    printf("Server exiting...\n");

    return 0;
}
