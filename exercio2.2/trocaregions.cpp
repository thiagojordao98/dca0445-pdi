#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat loadImage(const std::string& filename) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        std::cout << "nao abriu " << filename << std::endl;
        exit(-1);
    }
    return image;
}

cv::Rect getRegion(int x, int y, int width, int height) {
    return cv::Rect(x, y, width, height);
}

void swapRegions(cv::Mat& image, const cv::Rect& region1, const cv::Rect& region2) {
    cv::Mat temp;
    image(region1).copyTo(temp);
    image(region2).copyTo(image(region1));
    temp.copyTo(image(region2));
}

int main(int, char**) {
    cv::Mat image = loadImage("regions.png");
    cv::imshow("Original", image);

    int halfWidth = image.cols / 2;
    int halfHeight = image.rows / 2;

    cv::Rect topLeft = getRegion(0, 0, halfWidth, halfHeight);
    cv::Rect topRight = getRegion(halfWidth, 0, halfWidth, halfHeight);
    cv::Rect bottomLeft = getRegion(0, halfHeight, halfWidth, halfHeight);
    cv::Rect bottomRight = getRegion(halfWidth, halfHeight, halfWidth, halfHeight);

    swapRegions(image, topLeft, bottomRight);
    swapRegions(image, topRight, bottomLeft);

    cv::imshow("Troca regioes", image);
    cv::waitKey();
    return 0;
}
