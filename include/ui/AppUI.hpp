#ifndef APP_UI_HPP
#define APP_UI_HPP

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

class AppUI {
   public:
    AppUI();
    ~AppUI();

    bool openCamera(int cameraIndex = 0);
    void ShowVideoLoop();
    void Close();

   private:
    cv::VideoCapture _capture;
    cv::Mat _frame;
    bool _running;

    void processFrame(const cv::Mat &input, cv::Mat &output);
};

#endif
