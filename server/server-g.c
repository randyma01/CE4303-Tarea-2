#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // Configurar el puerto en el que el servidor escuchará conexiones
    int server_port = 12345; // Puedes cambiar esto al puerto que desees

    // Crear un socket para el servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
        exit(1);
    }

    // Configurar la estructura sockaddr_in para el servidor
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección y el puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error al vincular el socket a la dirección");
        exit(1);
    }

    // Escuchar por conexiones entrantes
    if (listen(server_socket, 5) == -1)
    {
        perror("Error al escuchar por conexiones entrantes");
        exit(1);
    }

    printf("El servidor está esperando conexiones...\n");

    while (1)
    {
        // Aceptar una conexión entrante
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            perror("Error al aceptar la conexión del cliente");
            exit(1);
        }

        // Leer datos del cliente
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1)
        {
            perror("Error al recibir datos del cliente");
            exit(1);
        }

        // Terminar la cadena recibida
        buffer[bytes_received] = '\0';

        // Mostrar los datos recibidos del cliente
        printf("Mensaje del cliente: %s\n", buffer);

        // Responder al cliente
        char response[] = "Hola, cliente!";
        if (send(client_socket, response, strlen(response), 0) == -1)
        {
            perror("Error al enviar datos al cliente");
            exit(1);
        }

        // Cerrar la conexión con el cliente
        close(client_socket);
    }

    // Cerrar el socket del servidor (esto generalmente no se alcanza en un servidor en funcionamiento)
    close(server_socket);

    return 0;
}
