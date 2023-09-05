#include <opencv2/opencv.hpp>

// Función para aplicar la ecualización de histograma a una imagen y sobrescribir la imagen de entrada
void ecualizarHistogramaYGuardar(const char* inputImagePath) {
    cv::Mat image = cv::imread(inputImagePath, cv::IMREAD_COLOR);

    if (image.empty()) {
        printf("No se pudo cargar la imagen.\n");
        return;
    }

    cv::Mat imageGray;
    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

    cv::Mat equalizedImage;
    cv::equalizeHist(imageGray, equalizedImage);

    cv::imwrite(inputImagePath, equalizedImage); // Sobrescribe la imagen de entrada
    printf("Imagen con histograma ecualizado sobrescrita en: %s\n", inputImagePath);
}

int main() {
    const char* inputImagePath = "prueba.jpg"; // Ruta de la imagen de entrada

    ecualizarHistogramaYGuardar(inputImagePath);

    return 0;
}
