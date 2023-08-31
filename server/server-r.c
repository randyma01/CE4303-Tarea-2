#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define MAX_FILENAME_LEN 256
#define MAX_PATH_LEN 256

// Función para crear la carpeta "processed images" si no existe
void createProcessedImagesFolder()
{
    struct stat st = {0};
    if (stat("processed images", &st) == -1)
    {
        if (mkdir("processed images", 0777) != 0)
        {
            perror("Error al crear la carpeta 'processed images'");
            exit(1);
        }
    }
}

int main()
{
    // Configurar el puerto en el que el servidor escuchará conexiones
    int server_port = 1717; // Cambia esto al puerto que desees

    createProcessedImagesFolder();

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

        // Leer el nombre del archivo de imagen enviado por el cliente
        char filename[MAX_FILENAME_LEN];
        int filename_len = recv(client_socket, filename, sizeof(filename), 0);
        if (filename_len == -1)
        {
            perror("Error al recibir el nombre del archivo");
            exit(1);
        }

        // Construir la ruta completa para guardar la imagen en "processed images"
        char filepath[MAX_PATH_LEN];
        snprintf(filepath, sizeof(filepath), "processed images/%s", filename);

        // Abrir un archivo para guardar la imagen recibida
        FILE *file = fopen(filepath, "wb");
        if (file == NULL)
        {
            perror("Error al abrir el archivo para escribir la imagen");
            exit(1);
        }

        // Recibir y guardar la imagen
        char buffer[1024];
        int bytes_received;
        while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
        {
            fwrite(buffer, 1, bytes_received, file);
        }

        // Cerrar el archivo
        fclose(file);

        // Enviar una respuesta al cliente
        char response[] = "Imagen recibida con éxito";
        if (send(client_socket, response, strlen(response), 0) == -1)
        {
            perror("Error al enviar respuesta al cliente");
            exit(1);
        }

        // Cerrar la conexión con el cliente
        close(client_socket);
    }

    // Cerrar el socket del servidor (esto generalmente no se alcanza en un servidor en funcionamiento)
    close(server_socket);

    return 0;
}
