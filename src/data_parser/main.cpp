#include "data_parser.h"

int main() {
    DataParser dataParser;

    dataParser.combineLogs(
            "data/orientation_events.txt",
            "data/keyboard_events.txt"
    );

    dataParser.getSlices();
}
