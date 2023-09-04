#include <opencv/cv.h>
#include <opencv/highgui.h>

int main() {
    // Cargar la imagen
    IplImage* image = cvLoadImage("imagen.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // Verificar si la imagen se ha cargado correctamente
    if (!image) {
        printf("No se pudo cargar la imagen.\n");
        return -1;
    }

    // Realizar la equalización de histogramas
    cvEqualizeHist(image, image);

    // Guardar la imagen igualada
    cvSaveImage("equalized_image.jpg", image);

    // Liberar la memoria
    cvReleaseImage(&image);

    printf("Imagen equalizada y guardada como 'equalized_image.jpg'.\n");

    return 0;
}