#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main()
{
    // Define the server and port to connect to
    char *server_ip = "172.19.110.38"; // Use "localhost" or "127.0.0.1" for local connection
    int port = 1717;                   // Change 1717 to the port you want to use

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

    // Loop to send messages to the server
    char message[1024];
    while (true)
    {
        printf("Enter a message to send to the server (or type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        // Remove the newline character at the end of the message
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n')
        {
            message[len - 1] = '\0';
        }

        // Check if the user wants to exit
        if (strcasecmp(message, "exit") == 0)
        {
            break;
        }

        // Send the message to the server
        if (send(client_socket, message, strlen(message), 0) == -1)
        {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }

        printf("Message sent: %s\n", message);

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
