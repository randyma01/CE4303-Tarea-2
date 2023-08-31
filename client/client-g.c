#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // Configurar la dirección IP y el puerto del servidor al que se conectará el cliente
    char *server_ip = "127.0.0.1"; // Cambia esto a la dirección IP del servidor
    int server_port = 1717;        // Cambia esto al puerto del servidor

    char file_name[256];

    while (1)
    {
        // Solicitar al usuario la ruta de la imagen que desea enviar
        printf("Ingrese la ruta de la imagen que desea enviar o escriba 'Exit' para salir: ");
        scanf("%s", file_name);

        // Verificar si el usuario desea salir
        if (strcmp(file_name, "Exit") == 0 || strcmp(file_name, "exit") == 0)
        {
            break; // Salir del bucle
        }

        // Abrir el archivo de imagen
        FILE *file = fopen(file_name, "rb");
        if (file == NULL)
        {
            perror("Error al abrir el archivo de imagen");
            continue; // Volver a solicitar la ruta de la imagen si hay un error
        }

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

        // Enviar el archivo de imagen al servidor
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
        {
            if (send(client_socket, buffer, bytes_read, 0) == -1)
            {
                perror("Error al enviar datos al servidor");
                exit(1);
            }
        }

        // Cerrar el archivo
        fclose(file);

        // Cerrar el socket del cliente
        close(client_socket);
    }

    return 0;
}
