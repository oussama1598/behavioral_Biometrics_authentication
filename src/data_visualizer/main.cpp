#include "data_parser/data_parser.h"
#include "data_visualizer.h"

namespace plt = matplotlibcpp;

int main() {
    DataParser user1DataParser{"user2"};

    user1DataParser.combineLogs(
            "data/user_2_orientation_events.txt",
            "data/user_2_keyboard_events.txt",
            "data/user_2_touch_events.txt"
    );

    user1DataParser.parseDataSlices();

    DataVisualizer dataVisualizer{user1DataParser};

    int vectorIndex = 12;

    dataVisualizer.visualizeOrientationHistogram(vectorIndex);
    dataVisualizer.visualizeOrientationTimeHistogram(vectorIndex);
    dataVisualizer.visualizeKeystrokeDurationHistogram(vectorIndex);
    dataVisualizer.visualizeDistributionOfActionsOnTheScreenHistogram(vectorIndex);
    dataVisualizer.visualizeMovementDirectionHistogram(vectorIndex);
    dataVisualizer.visualizeAverageSpeedPerMovementDirectionHistogram(vectorIndex);
    dataVisualizer.visualizeTravelDistanceHistogram(vectorIndex);
    dataVisualizer.visualizeExtremeMovementSpeedRelativeToTravelDistanceHistogram(vectorIndex);
    dataVisualizer.visualizeMovementElapsedTimeHistogram(vectorIndex);
    dataVisualizer.visualizeAverageMovementSpeedRelativetoTravelDistanceHistogram(vectorIndex);

    return 0;
}
