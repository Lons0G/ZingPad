#include "calib/Homography.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

bool Homography::overleayImageOnQuad(cv::Mat &frame, const cv::Mat &overlay,
                                     const std::vector<cv::Point2d> &quad) {
    // PUNTOS DEL MARCADOR
    std::vector<cv::Point2d> srcpoints = {
        {0, 0},
        {static_cast<float>(overlay.cols - 1), 0},
        {static_cast<float>(overlay.cols - 1), static_cast<float>(overlay.rows - 1)},
        {0, static_cast<float>(overlay.rows - 1)}};

    // CALCULAR HOMOGRAFIA
    cv::Mat H = cv::findHomography(srcpoints, quad);
    if (H.empty()) {
        return false;
    }

    // IMAGEN DEFORMADA
    cv::Mat warped;
    cv::warpPerspective(overlay, warped, H, frame.size());

    // CREAR MASCARA DE LA PROYECCION
    cv::Mat mask;
    cv::cvtColor(warped, mask, cv::COLOR_BGR2GRAY);
    cv::threshold(mask, mask, 1, 255, cv::THRESH_BINARY);

    return true;
}
