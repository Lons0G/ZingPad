#include "core/ImageOverlay.hpp"

#include <iostream>
#include <opencv2/core/persistence.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

#include "calib/Homography.hpp"

ImageOverlay::ImageOverlay() {}

bool ImageOverlay::loadImage(const std::string &path) {
    _image = cv::imread(path);

    if (_image.empty()) {
        std::cout << "Error: No se pudo cargar la imagen" << std::endl;
        return false;
    }

    return true;
}

bool ImageOverlay::overlay(cv::Mat &frame, const std::vector<cv::Point2f> &quad) {
    if (_image.empty()) {
        std::cout << "Error: No hay una imagen cargada" << std::endl;
        return false;
    }

    return Homography::overleayImageOnQuad(frame, _image, quad);
}
