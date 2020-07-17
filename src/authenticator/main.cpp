#include "authenticator.h"

int main() {
    DataParser user1DataParser{"user1"};
    DataParser user2DataParser{"user2"};

    user1DataParser.combineLogs(
            "data/user_1_orientation_events.txt",
            "data/user_1_keyboard_events.txt",
            "data/user_1_touch_events.txt"
    );

    user2DataParser.combineLogs(
            "data/user_2_orientation_events.txt",
            "data/user_2_keyboard_events.txt",
            "data/user_2_touch_events.txt"
    );

    user1DataParser.parseDataSlices();
    user2DataParser.parseDataSlices();

    Authenticator authenticator{user1DataParser, user2DataParser};

    authenticator.train();

    std::cout << authenticator.authenticate(user1DataParser.getDataVectors()[5]) << std::endl;
}
