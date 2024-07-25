#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

// Função para calcular os momentos de Hu de uma imagem
std::vector<double> calculateHuMoments(const cv::Mat& image) {
    double huMoments[7];
    cv::Moments moment = cv::moments(image, false);
    cv::HuMoments(moment, huMoments);

    std::vector<double> huMomentsVector(7);
    for (int i = 0; i < 7; i++) {
        huMomentsVector[i] = -1 * std::copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
    }
    return huMomentsVector;
}

// Função para calcular a distância euclidiana entre dois vetores
double calculateEuclideanDistance(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double distance = 0.0;
    for (size_t i = 0; i < vec1.size(); i++) {
        distance += std::pow(vec1[i] - vec2[i], 2);
    }
    return std::sqrt(distance);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <person_image> <crowd_image>" << std::endl;
        return -1;
    }
  
    cv::Mat personImage = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    cv::Mat crowdImage = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);

    if (personImage.empty() || crowdImage.empty()) {
        std::cerr << "Could not open or find the images!" << std::endl;
        return -1;
    }

    // Redimensionar as imagens para acelerar o processamento
    double scaleFactor = 0.5; // Ajuste conforme necessário
    cv::resize(personImage, personImage, cv::Size(), scaleFactor, scaleFactor);
    cv::resize(crowdImage, crowdImage, cv::Size(), scaleFactor, scaleFactor);

    // Calcular os momentos de Hu da imagem da pessoa
    std::vector<double> personHuMoments = calculateHuMoments(personImage);

    int crowdRows = crowdImage.rows;
    int crowdCols = crowdImage.cols;
    int personRows = personImage.rows;
    int personCols = personImage.cols;

    double minDistance = DBL_MAX;
    cv::Rect bestMatch;

    // Deslizar a janela sobre a imagem da multidão
    for (int y = 0; y <= crowdRows - personRows; y++) {
        for (int x = 0; x <= crowdCols - personCols; x++) {
            cv::Mat roi = crowdImage(cv::Rect(x, y, personCols, personRows));
            std::vector<double> roiHuMoments = calculateHuMoments(roi);
            double distance = calculateEuclideanDistance(personHuMoments, roiHuMoments);

            if (distance < minDistance) {
                minDistance = distance;
                bestMatch = cv::Rect(x, y, personCols, personRows);
            }
        }
    }

    // Desenhar o melhor retângulo de correspondência na imagem da multidão
    cv::Mat resultImage;
    cv::cvtColor(crowdImage, resultImage, cv::COLOR_GRAY2BGR);
    cv::rectangle(resultImage, bestMatch, CV_RGB(255, 0, 0), 2);

    cv::imshow("Best Match", resultImage);
    cv::waitKey(0);

    return 0;
}
