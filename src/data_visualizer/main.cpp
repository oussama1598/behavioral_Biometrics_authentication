#include "data_parser/data_parser.h"
#include "data_visualizer.h"

namespace plt = matplotlibcpp;

int main() {
    DataParser user1DataParser{"user1"};

    user1DataParser.combineLogs(
            "data/user_2_orientation_events.txt",
            "data/user_2_keyboard_events.txt",
            "data/user_2_touch_events.txt"
    );

    user1DataParser.parseDataSlices();

    DataVisualizer dataVisualizer{user1DataParser};

    dataVisualizer.visualizeOrientationHistogram(1);
    dataVisualizer.visualizeOrientationTimeHistogram(1);

    return 0;
}
