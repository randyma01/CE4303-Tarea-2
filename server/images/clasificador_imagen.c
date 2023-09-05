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

    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(5, 5), 0);

    cv::Mat hsvImage;
    cv::cvtColor(blurred, hsvImage, cv::COLOR_BGR2HSV);

    cv::Scalar lowerBlue = cv::Scalar(90, 50, 50);
    cv::Scalar upperBlue = cv::Scalar(130, 255, 255);

    cv::Scalar lowerGreen = cv::Scalar(35, 50, 50);
    cv::Scalar upperGreen = cv::Scalar(85, 255, 255);

    cv::Scalar lowerRed1 = cv::Scalar(0, 50, 50);
    cv::Scalar upperRed1 = cv::Scalar(10, 255, 255);
    cv::Scalar lowerRed2 = cv::Scalar(160, 50, 50);
    cv::Scalar upperRed2 = cv::Scalar(180, 255, 255);

    cv::Mat maskBlue, maskGreen, maskRed;
    cv::inRange(hsvImage, lowerBlue, upperBlue, maskBlue);
    cv::inRange(hsvImage, lowerGreen, upperGreen, maskGreen);
    cv::inRange(hsvImage, lowerRed1, upperRed1, maskRed);
    cv::inRange(hsvImage, lowerRed2, upperRed2, maskRed);
    cv::bitwise_or(maskRed, maskRed, maskRed);

    int bluePixels = cv::countNonZero(maskBlue);
    int greenPixels = cv::countNonZero(maskGreen);
    int redPixels = cv::countNonZero(maskRed);

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
