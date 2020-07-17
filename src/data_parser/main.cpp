#include "data_parser.h"

int main() {
    DataParser dataParser("user_2");

    dataParser.combineLogs(
            "data/user_2_orientation_events.txt",
            "data/user_2_keyboard_events.txt",
            "data/user_2_touch_events.txt"
    );

    dataParser.parseDataSlices();

    std::vector<std::vector<double>> data = dataParser.getDataVectors();

    std::cout << data.size() << std::endl;

    return 0;
}
