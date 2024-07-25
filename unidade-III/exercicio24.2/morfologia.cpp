#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

void preFiltragem(const cv::Mat& img, const std::string& output_name, const cv::Mat& str) {
    cv::Mat dilatada, erodida;

    // Aplicação da operação de fechamento morfológico: primeiro dilatar e depois erodir
    cv::dilate(img, dilatada, str);
    cv::erode(dilatada, erodida, str);

    // Reverter a imagem de volta ao normal
    cv::Mat resultado;
    cv::bitwise_not(erodida, resultado);

    // Salvar a imagem processada com o nome especificado
    cv::imwrite(output_name, resultado);
}

int main() {
    std::vector<std::string> filenames = {
        "digitos-1.png",
        "digitos-2.png",
        "digitos-3.png",
        "digitos-4.png",
        "digitos-5.png"
    };

    // Elemento estruturante
    cv::Mat str = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 15));

    for (size_t i = 0; i < filenames.size(); ++i) {
        cv::Mat img = cv::imread(filenames[i], cv::IMREAD_UNCHANGED);

        if (img.empty()) {
            std::cerr << "Erro ao carregar a imagem: " << filenames[i] << std::endl;
            continue;
        }

        // Inverter as cores para trabalhar com imagem binária
        cv::bitwise_not(img, img);

        // Processar a imagem e salvar o resultado
        preFiltragem(img, "segmentos-fechados" + std::to_string(i + 1) + ".png", str);
    }

    return 0;
}
