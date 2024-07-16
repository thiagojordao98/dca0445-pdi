#include <iostream>
#include <opencv2/opencv.hpp>

void getCoordinates(const std::string& pointName, int& x, int& y) {
    std::cout << "Digite as coordenadas do ponto " << pointName << "(x, y):" << std::endl;
    std::cin >> x >> y;
}

void invertColorsInRegion(cv::Mat& image, int minX, int maxX, int minY, int maxY) {
    for (int i = minX; i < maxX; i++) {
        for (int j = minY; j < maxY; j++) {
            cv::Vec3b& pixel = image.at<cv::Vec3b>(i, j);
            pixel[0] = 255 - pixel[0]; // Inverte a cor B
            pixel[1] = 255 - pixel[1]; // Inverte a cor G
            pixel[2] = 255 - pixel[2]; // Inverte a cor R
        }
    }
}

int main(int, char**) {
    cv::Mat image = cv::imread("./regions.png", cv::IMREAD_COLOR);
  
    if (!image.data) {
        std::cout << "nao abriu regions.png" << std::endl;
        return -1;
    }

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

    int x1, y1, x2, y2;
    getCoordinates("P1", x1, y1);
    getCoordinates("P2", x2, y2);

    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);

    invertColorsInRegion(image, minX, maxX, minY, maxY);

    cv::imwrite("regions.png", image);
    cv::imshow("janela", image);
    cv::waitKey();
    return 0;
}
