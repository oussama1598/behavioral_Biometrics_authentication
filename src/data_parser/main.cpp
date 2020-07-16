#include "data_parser.h"

int main() {
    DataParser dataParser;

    dataParser.combineLogs(
            "data/user_1_orientation_events.txt",
            "data/user_1_keyboard_events.txt",
            "data/user_1_touch_events.txt"
    );

    dataParser.parseDataSlices();

    std::vector<std::vector<double>> data = dataParser.getDataVectors();

    std::cout << data.size() << std::endl;

    return 0;
}
