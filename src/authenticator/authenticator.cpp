#include "authenticator.h"

Authenticator::Authenticator(int currentUserNumberOfSlices, int unknownNumberOfSlices,
                             DataParser &currentUserDataParser,
                             DataParser &unknownUserDataParser) :
        _currentUserNumberOfSlices(currentUserNumberOfSlices),
        _unknownNumberOfSlices(unknownNumberOfSlices) {
    _currentUserDataVectors = currentUserDataParser.getDataVectors();
    _unknownUserDataVectors = unknownUserDataParser.getDataVectors();

    _createSVMModel();
    _createTrainingData();
}

void Authenticator::_createSVMModel() {
    _svm = cv::ml::SVM::create();

    _svm->setType(cv::ml::SVM::C_SVC);
    _svm->setKernel(cv::ml::SVM::RBF);
    _svm->setDegree(3);
    _svm->setC(1);
    _svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 1000, 1e-10));
}

void Authenticator::_createTrainingData() {
    _trainingMatrix = cv::Mat(
            _currentUserNumberOfSlices + _unknownNumberOfSlices,
            (int) _currentUserDataVectors[0].size(),
            CV_32F
    );

    for (size_t i = 0; i < (size_t) _currentUserNumberOfSlices; ++i) {
        for (size_t j = 0; j < _currentUserDataVectors[i].size(); ++j) {
            _trainingMatrix.at<float>(i, j) = _currentUserDataVectors[i][j];
        }

        _dataLabels.push_back(1);
    }

    for (size_t i = 0; i < (size_t) _unknownNumberOfSlices; ++i) {
        int offsetI = _currentUserNumberOfSlices + i;

        for (size_t j = 0; j < _unknownUserDataVectors[i].size(); ++j) {
            _trainingMatrix.at<float>(offsetI, j) = _unknownUserDataVectors[i][j];
        }

        _dataLabels.push_back(-1);
    }

    _dataLabels.convertTo(_dataLabels, CV_32SC1);
}


void Authenticator::train() {
    _svm->train(_trainingMatrix, cv::ml::ROW_SAMPLE, _dataLabels);
}

float Authenticator::authenticate(const std::vector<double> &sample) {
    cv::Mat sampleMatrix(
            1, 122, CV_32F
    );

    for (size_t j = 0; j < sample.size(); ++j) {
        sampleMatrix.at<float>(0, j) = sample.at(j);
    }

    return _svm->predict(sampleMatrix);
}
