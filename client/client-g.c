#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // Configurar la dirección IP y el puerto del servidor al que se conectará el cliente
    char *server_ip = "127.0.0.1"; // Cambia esto a la dirección IP del servidor
    int server_port = 12345;       // Cambia esto al puerto del servidor

    // Crear un socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // Configurar la estructura sockaddr_in para la conexión al servidor
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Error en la configuración de la dirección");
        exit(1);
    }

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error al conectar al servidor");
        exit(1);
    }

    // Enviar datos al servidor
    char message[] = "Hola, servidor!";
    if (send(client_socket, message, strlen(message), 0) == -1)
    {
        perror("Error al enviar datos al servidor");
        exit(1);
    }

    // Recibir datos del servidor
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
    {
        perror("Error al recibir datos del servidor");
        exit(1);
    }

    // Terminar la cadena recibida
    buffer[bytes_received] = '\0';

    // Mostrar los datos recibidos del servidor
    printf("Mensaje del servidor: %s\n", buffer);

    // Cerrar el socket del cliente
    close(client_socket);

    return 0;
}
