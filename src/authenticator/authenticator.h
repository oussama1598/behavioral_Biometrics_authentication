#pragma once

#include <opencv2/opencv.hpp>
#include "data_parser/data_parser.h"


class Authenticator {
private:
    int _currentUserNumberOfSlices, _unknownNumberOfSlices;

    std::vector<std::vector<double>> _currentUserDataVectors;
    std::vector<std::vector<double>> _unknownUserDataVectors;

    cv::Ptr<cv::ml::SVM> _svm;

    cv::Mat _trainingMatrix;
    cv::Mat _dataLabels;

private:
    void _createSVMModel();

public:
    Authenticator(int currentUserNumberOfSlices, int unknownNumberOfSlices,
                  DataParser &currentUserDataParser, DataParser &unknownUserDataParser);

    void train();

    float authenticate(const std::vector<double> &);

    void _createTrainingData();
};

