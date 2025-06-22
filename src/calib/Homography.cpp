#include "calib/Homography.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

bool Homography::overlayImageOnQuad(cv::Mat &frame, const cv::Mat &overlay,
                                    const std::vector<cv::Point2f> &quad) {
    std::vector<cv::Point2f> srcpoints = {
        {0, 0},
        {static_cast<float>(overlay.cols - 1), 0},
        {static_cast<float>(overlay.cols - 1), static_cast<float>(overlay.rows - 1)},
        {0, static_cast<float>(overlay.rows - 1)}};

    cv::Mat H = cv::findHomography(srcpoints, quad);
    if (H.empty()) {
        std::cout << "[DEBUG] Homografía vacía" << std::endl;
        return false;
    }

    // Imagen deformada
    cv::Mat warped;
    cv::warpPerspective(overlay, warped, H, frame.size());

    // Máscara para saber qué regiones ocupar
    cv::Mat mask;
    cv::cvtColor(warped, mask, cv::COLOR_BGR2GRAY);
    cv::threshold(mask, mask, 1, 255, cv::THRESH_BINARY);

    // Combinar imagen con fondo
    warped.copyTo(frame, mask);  // <- ESTA LÍNEA ES CRUCIAL

    return true;
}
