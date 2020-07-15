#include "data_parser.h"

int main() {
    DataParser dataParser;

    dataParser.combineLogs(
            "data/orientation_events.txt",
            "data/keyboard_events.txt",
            "data/touch_events.txt"
    );

    dataParser.parseDataSlices();
}
