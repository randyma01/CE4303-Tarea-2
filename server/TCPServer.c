#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PORT 1717
#define BUFFER_SIZE 1024

// Función para determinar el color predominante en la imagen
const char* determinarColorPredominante(const char* filename) {
    cv::Mat image = cv::imread(filename);

    if (image.empty()) {
        perror("Error al cargar la imagen");
        return "Desconocido";
    }

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    cv::Scalar lowerRed = cv::Scalar(0, 100, 100);
    cv::Scalar upperRed = cv::Scalar(10, 255, 255);

    cv::Scalar lowerGreen = cv::Scalar(35, 100, 100);
    cv::Scalar upperGreen = cv::Scalar(85, 255, 255);

    cv::Scalar lowerBlue = cv::Scalar(100, 100, 100);
    cv::Scalar upperBlue = cv::Scalar(130, 255, 255);

    cv::Mat maskRed, maskGreen, maskBlue;
    cv::inRange(hsvImage, lowerRed, upperRed, maskRed);
    cv::inRange(hsvImage, lowerGreen, upperGreen, maskGreen);
    cv::inRange(hsvImage, lowerBlue, upperBlue, maskBlue);

    int redPixels = cv::countNonZero(maskRed);
    int greenPixels = cv::countNonZero(maskGreen);
    int bluePixels = cv::countNonZero(maskBlue);

    if (redPixels > greenPixels && redPixels > bluePixels) {
        return "images/rojos";
    } else if (greenPixels > redPixels && greenPixels > bluePixels) {
        return "images/verdes";
    } else {
        return "images/azules";
    }
}

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

        // Determinar el color predominante
        const char* colorPredominante = determinarColorPredominante(filename);
        printf("Color predominante: %s\n", colorPredominante);
        
        char command[100];
        snprintf(command, sizeof(command), "mv %s %s/", filename, colorPredominante);
    	system(command);

        close(client_fd);
    }

    close(server_fd);

    return 0;
}

