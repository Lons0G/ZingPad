#include "ui/AppUI.hpp"

#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "core/ImageOverlay.hpp"

AppUI::AppUI() : _running(false) {
    _overlay_ready = _overlay.loadImage("assets/samples/asesora.png");
    if (!_overlay_ready) {
        std::cerr << "No se pudo cargar la imagen de overlay" << std::endl;
    }
}

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
    bool fullscreen = false;

    cv::namedWindow("ZingPad", cv::WINDOW_NORMAL);

    while (_running) {
        _capture >> _frame;
        if (_frame.empty())
            break;

        cv::Mat processed;
        processFrame(_frame, processed);

        cv::imshow("ZingPad", processed);

        char key = (char)cv::waitKey(1);
        if (key == 27 || key == 'q') {
            _running = false;
        } else if (key == 32) {  // Tecla espacio
            fullscreen = !fullscreen;
            cv::setWindowProperty("ZingPad", cv::WND_PROP_FULLSCREEN,
                                  fullscreen ? cv::WINDOW_FULLSCREEN : cv::WINDOW_NORMAL);
            std::cout << "[DEBUG] Cambio de pantalla: "
                      << (fullscreen ? "Pantalla completa" : "Ventana") << std::endl;
        }
    }
}

void AppUI::processFrame(const cv::Mat &input, cv::Mat &output) {
    input.copyTo(output);

    cv::Mat gray, edges;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    double maxArea = 0.0;
    std::vector<cv::Point> bestApprox;

    for (const auto &contour : contours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * cv::arcLength(contour, true), true);

        if (approx.size() == 4 && cv::isContourConvex(approx)) {
            double area = cv::contourArea(approx);
            if (area > 5000 && area > maxArea) {
                maxArea = area;
                bestApprox = approx;
            }
        }
    }

    // Parámetro de suavizado
    const float alpha = 0.2f;

    if (!bestApprox.empty()) {
        std::vector<cv::Point2f> quad;
        for (const auto &pt : bestApprox) {
            quad.push_back(cv::Point2f(pt));
        }

        // Si ya teníamos un marcador previo, suavizamos hacia la nueva posición
        if (!_lastQuad.empty() && _lastQuad.size() == 4) {
            for (int i = 0; i < 4; ++i) {
                _lastQuad[i] = (1.0f - alpha) * _lastQuad[i] + alpha * quad[i];
            }
        } else {
            _lastQuad = quad;
        }

        _framesWithoutQuad = 0;
        std::cout << "[DEBUG] Cuadrado suavizado actualizado" << std::endl;
    } else if (!_lastQuad.empty() && _framesWithoutQuad < _maxFramesHold) {
        _framesWithoutQuad++;
        std::cout << "[DEBUG] Usando último cuadrado suavizado (sin detección)" << std::endl;
    } else {
        _lastQuad.clear();
    }

    // Proyección y dibujo si tenemos un cuadrado válido
    if (!_lastQuad.empty()) {
        std::vector<cv::Point> intQuad;
        for (const auto &pt : _lastQuad) {
            intQuad.push_back(cv::Point(cvRound(pt.x), cvRound(pt.y)));
        }

        cv::polylines(output, intQuad, true, cv::Scalar(255, 0, 0), 3);
        _overlay.overlay(output, _lastQuad);
    }
}

void AppUI::Close() {
    if (_capture.isOpened()) {
        _capture.release();
    }
    cv::destroyAllWindows();
}
