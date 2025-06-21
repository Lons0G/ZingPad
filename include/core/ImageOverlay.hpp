#ifndef IMAGEOVERLAY_HPP
#define IMAGEOVERLAY_HPP

#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class ImageOverlay {
   public:
    ImageOverlay();

    // CARGAR IMAGEN
    bool loadImage(const std::string &path);

    // PROYECTAR IMAGEN SOBRE EL MARCADOR
    bool overlay(cv::Mat &frame, const std::vector<cv::Point2f> &quad);

   private:
    cv::Mat _image;
};

#endif
