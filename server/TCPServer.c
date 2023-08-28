#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
 
#define PORT 1717
#define BUFFER_SIZE 1024 
 
int main() { 
    int server_fd, client_fd; 
    struct sockaddr_in server_addr, client_addr; 
    socklen_t client_addr_len = sizeof(client_addr); 
 
    // Crear socket 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("Error al crear el socket"); 
        exit(EXIT_FAILURE); 
    } 
 
    // Configurar la dirección del servidor 
    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
 
    // Vincular el socket a la dirección 
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) { 
        perror("Error en el enlace"); 
        exit(EXIT_FAILURE); 
    } 
 
    // Escuchar conexiones entrantes 
    if (listen(server_fd, 5) == -1) { 
        perror("Error al escuchar"); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Servidor esperando conexiones en el puerto %d...\n", PORT); 
 
    while (1) { 
        // Aceptar la conexión del cliente 
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) { 
            perror("Error al aceptar la conexión del cliente"); 
            continue; 
        } 
 
        printf("Cliente conectado desde %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); 
 
        // Recibir datos (imagen) del cliente 
        char buffer[BUFFER_SIZE]; 
        FILE *image_file; 
        int bytes_received, total_bytes_received = 0; 
        char filename[50]; 
        sprintf(filename, "images/imagen_%d.jpg", ntohs(client_addr.sin_port)); // Generar un nombre único para la imagen 
         
        image_file = fopen(filename, "wb"); // Abrir archivo en modo escritura binaria 
 
        if (image_file == NULL) { 
            perror("Error al abrir el archivo para escritura"); 
            close(client_fd); 
            continue; 
        } 
 
        while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) { 
            fwrite(buffer, 1, bytes_received, image_file); 
            total_bytes_received += bytes_received; 
        } 
 
        fclose(image_file); 
        printf("Imagen recibida y guardada como %s (%d bytes)\n", filename, total_bytes_received); 
 
        close(client_fd); 
    } 
 
    close(server_fd); 
 
    return 0; 
}