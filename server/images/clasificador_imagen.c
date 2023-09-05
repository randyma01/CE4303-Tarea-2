#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <ruta_de_la_imagen>\n", argv[0]);
        return 1;
    }

    cv::Mat image = cv::imread(argv[1]);

    if (image.empty()) {
        printf("No se pudo cargar la imagen desde la ruta especificada.\n");
        return 1;
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

    const char* colorPredominante;
    if (redPixels > greenPixels && redPixels > bluePixels) {
        colorPredominante = "rojas";
    } else if (greenPixels > redPixels && greenPixels > bluePixels) {
        colorPredominante = "verdes";
    } else {
        colorPredominante = "azules";
    }

    char command[100];
    snprintf(command, sizeof(command), "mv %s %s/", argv[1], colorPredominante);
    system(command);

    printf("La imagen se ha clasificado como \"%s\" y se ha movido al directorio correspondiente.\n", colorPredominante);

    return 0;
}
