#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

const int SIDE = 256;
const int PERIODOS = 8;

cv::Mat generateSineImage(int side, int periodos) {
    cv::Mat image = cv::Mat::zeros(side, side, CV_32FC1);
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            image.at<float>(i, j) = 127 * sin(2 * M_PI * periodos * j / side) + 128;
        }
    }
    return image;
}

void saveImage(const cv::Mat& image, const std::string& filename) {
    cv::Mat normalizedImage;
    cv::normalize(image, normalizedImage, 0, 255, cv::NORM_MINMAX);
    normalizedImage.convertTo(normalizedImage, CV_8U);
    cv::imwrite(filename, normalizedImage);
}

int main(int argc, char** argv) {
    std::stringstream ss_img, ss_yml;
    ss_yml << "senoide-" << SIDE << ".yml";
    ss_img << "senoide-" << SIDE << ".png";

    cv::Mat image = generateSineImage(SIDE, PERIODOS);

    cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);
    fs << "mat" << image;
    fs.release();

    saveImage(image, ss_img.str());

    fs.open(ss_yml.str(), cv::FileStorage::READ);
    fs["mat"] >> image;

    saveImage(image, ss_img.str());

    cv::imshow("image", image);
    cv::waitKey();

    return 0;
}
