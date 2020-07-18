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

    dataVisualizer.visualizeOrientationHistogram();
    dataVisualizer.visualizeOrientationTimeHistogram();
    dataVisualizer.visualizeDistributionOfActionsOnTheScreenHistogram();
    dataVisualizer.visualizeDistributionOfActionsOnTheScreenHistogram();
    dataVisualizer.visualizeMovementDirectionHistogram();
    dataVisualizer.visualizeAverageSpeedPerMovementDirectionHistogram();
    dataVisualizer.visualizeTravelDistanceHistogram();
    dataVisualizer.visualizeExtremeMovementSpeedRelativeToTravelDistanceHistogram();
    dataVisualizer.visualizeMovementElapsedTimeHistogram();
    dataVisualizer.visualizeAverageMovementSpeedRelativetoTravelDistanceHistogram();

    return 0;
}
