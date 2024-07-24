#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat image, gray;
  std::ofstream file;
  int pontosSimples = 0;
  int pontosCompleto = 0;

  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "nao abriu " << argv[1] << std::endl;
    return 0;
  }

  cv::threshold(image, image, 1, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  // Encontre contornos usando CHAIN_APPROX_SIMPLE
  cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  cv::Mat imageSimpleContours;
  cv::cvtColor(image, imageSimpleContours, cv::COLOR_GRAY2BGR);

  file.open("contornos_simples.svg");
  if (!file.is_open()) {
    std::cout << "nao abriu contornos_simples.svg" << std::endl;
    return 0;
  }

  file << "<svg height=\"" << image.rows << "\" width=\"" << image.cols
       << "\" xmlns=\"http://www.w3.org/2000/svg\"> " << std::endl;

  for (size_t i = 0; i < contours.size(); i++) {
    file << "<path d=\"M " << contours[i][0].x << " " << contours[i][0].y
         << " ";
    for (size_t j = 1; j < contours[i].size(); j++) {
      file << "L" << contours[i][j].x << " " << contours[i][j].y << " ";
    }
    file << "Z\" fill=\"#cccccc\" stroke=\"black\" stroke-width=\"1\" />"
         << std::endl;
    cv::drawContours(imageSimpleContours, contours, i, CV_RGB(255, 0, 0));
    pontosSimples += contours[i].size();
  }
  file << "</svg>" << std::endl;
  file.close();

  // Encontre contornos usando CHAIN_APPROX_NONE
  cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

  cv::Mat imageCompleteContours;
  cv::cvtColor(image, imageCompleteContours, cv::COLOR_GRAY2BGR);

  file.open("contornos_completos.svg");
  if (!file.is_open()) {
    std::cout << "nao abriu contornos_completos.svg" << std::endl;
    return 0;
  }

  file << "<svg height=\"" << image.rows << "\" width=\"" << image.cols
       << "\" xmlns=\"http://www.w3.org/2000/svg\"> " << std::endl;

  for (size_t i = 0; i < contours.size(); i++) {
    file << "<path d=\"M " << contours[i][0].x << " " << contours[i][0].y
         << " ";
    for (size_t j = 1; j < contours[i].size(); j++) {
      file << "L" << contours[i][j].x << " " << contours[i][j].y << " ";
    }
    file << "Z\" fill=\"#cccccc\" stroke=\"black\" stroke-width=\"1\" />"
         << std::endl;
    cv::drawContours(imageCompleteContours, contours, i, CV_RGB(255, 0, 0));
    pontosCompleto += contours[i].size();
  }
  file << "</svg>" << std::endl;
  file.close();

  std::cout << "Numero de pontos gerados (CHAIN_APPROX_SIMPLE): " << pontosSimples << "\n";
  std::cout << "Numero de pontos gerados (CHAIN_APPROX_NONE): " << pontosCompleto << "\n";

  cv::imshow("imagem - contornos simples", imageSimpleContours);
  cv::imshow("imagem - contornos completos", imageCompleteContours);
  cv::waitKey();
  return 0;
}
