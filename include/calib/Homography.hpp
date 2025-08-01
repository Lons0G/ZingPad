#ifndef HOMOGRAPHY_HPP
#define HOMOGRAPHY_HPP

#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

// CLASE HOMOGRAFIA
class Homography {
   public:
    static bool overlayImageOnQuad(cv::Mat &frame, const cv::Mat &overlay,
                                   const std::vector<cv::Point2f> &quad);
};

#endif
