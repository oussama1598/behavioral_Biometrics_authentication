#include "data_visualizer.h"

DataVisualizer::DataVisualizer(DataParser &dataParser) {
    _dataVectors = dataParser.getDataVectors();
}

void DataVisualizer::_visualize(const std::string &title, int vectorIndex, int start, int finish) {
    std::vector<double> x;
    std::vector<double> y;

    for (int i = start, j = 0; i < finish; ++i, ++j) {
        x.push_back(j);
        y.push_back(_dataVectors[vectorIndex][i]);
    }

    plt::clf();
    plt::bar(x, y);
    plt::title(title);
    plt::show();
}

void DataVisualizer::visualizeOrientationHistogram(int vectorIndex) {
    _visualize("Orientation Histogram", vectorIndex, 0, 4);
}

void DataVisualizer::visualizeOrientationTimeHistogram(int vectorIndex) {
    _visualize("Orientation Time Histogram", vectorIndex, 4, 8);
}
