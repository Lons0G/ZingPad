#ifndef APP_UI_HPP
#define APP_UI_HPP

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

#include "core/ImageOverlay.hpp"

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

    std::vector<cv::Point2f> _lastQuad;  // Último cuadrado válido
    int _framesWithoutQuad = 0;          // Cuántos frames no se ha detectado
    const int _maxFramesHold = 0;        // Cuántos frames aguanta sin detección

    ImageOverlay _overlay;
    bool _overlay_ready = false;
    void processFrame(const cv::Mat &input, cv::Mat &output);
};

#endif
