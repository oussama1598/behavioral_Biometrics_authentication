#pragma once

#include "data_parser/data_parser.h"
#include "lib/matplotlibcpp.h"

namespace plt = matplotlibcpp;

class DataVisualizer {
private:
    std::vector<std::vector<double>> _dataVectors;

private:
    void _visualize(const std::string &title, int vectorIndex, int start, int finish);

public:

    explicit DataVisualizer(DataParser &dataParser);

    void visualizeOrientationHistogram(int vectorIndex = 0);

    void visualizeOrientationTimeHistogram(int vectorIndex = 0);
};

