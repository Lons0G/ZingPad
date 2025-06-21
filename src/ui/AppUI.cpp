#include "ui/AppUI.hpp"

#include <iostream>

AppUI::AppUI() : _running(false) {}

AppUI::~AppUI() { Close(); }

bool AppUI::openCamera(int cameraIndex) {
    _capture.open(cameraIndex);
    if (!_capture.isOpened()) {
        std::cerr << "Error: No se pudo abrir la cámara." << std::endl;
        return false;
    }
    return true;
}

void AppUI::ShowVideoLoop() {
    if (!_capture.isOpened())
        return;

    _running = true;

    cv::namedWindow("ZingPad", cv::WINDOW_NORMAL);
    cv::setWindowProperty("ZingPad", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    while (_running) {
        _capture >> _frame;
        if (_frame.empty())
            break;

        cv::Mat processed;
        processFrame(_frame, processed);

        cv::imshow("ZingPad", processed);

        // TECLA ESC PARA SALIR
        char key = (char)cv::waitKey(1);
        if (key == 27 || key == 'q') {
            _running = false;
        }
    }
}

void AppUI::processFrame(const cv::Mat& input, cv::Mat& output) { input.copyTo(output); }

void AppUI::Close() {
    if (_capture.isOpened()) {
        _capture.release();
    }
    cv::destroyAllWindows();
}
