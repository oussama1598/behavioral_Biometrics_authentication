#include "data_visualizer.h"

DataVisualizer::DataVisualizer(DataParser &dataParser) {
    _dataVectors = dataParser.getDataVectors();
}

void DataVisualizer::_visualize(const std::string &title, int vectorIndex, int start, int count) {
    std::vector<double> x;
    std::vector<double> y;

    for (int i = start, j = 0; i < start + count; ++i, ++j) {
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
    _visualize("Orientation Time Histogram", vectorIndex, 4, 4);
}

void DataVisualizer::visualizeDistributionOfActionsOnTheScreenHistogram(int vectorIndex) {
    _visualize("Distribution of Actions on the Screen Histogram", vectorIndex, 61, 9);
}

void DataVisualizer::visualizeMovementDirectionHistogram(int vectorIndex) {
    _visualize("Movement Direction Histogram", vectorIndex, 70, 8);
}

void DataVisualizer::visualizeAverageSpeedPerMovementDirectionHistogram(int vectorIndex) {
    _visualize("Average Speed per Movement Direction Histogram", vectorIndex, 78, 8);
}

void DataVisualizer::visualizeTravelDistanceHistogram(int vectorIndex) {
    _visualize("Travel Distance Histogram", vectorIndex, 86, 9);
}

void
DataVisualizer::visualizeExtremeMovementSpeedRelativeToTravelDistanceHistogram(int vectorIndex) {
    _visualize("Extreme Movement Speed Relative to Travel Distance Histogram", vectorIndex, 95, 9);
}

void DataVisualizer::visualizeMovementElapsedTimeHistogram(int vectorIndex) {
    _visualize("Movement Elapsed Time Histogram", vectorIndex, 104, 9);
}


void
DataVisualizer::visualizeAverageMovementSpeedRelativetoTravelDistanceHistogram(int vectorIndex) {
    _visualize("Average Movement Speed Relative to Travel Distance", vectorIndex, 113, 9);
}
