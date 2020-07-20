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

    Authenticator authenticator{15, 15, user2DataParser, user1DataParser};

    authenticator.train();

    std::cout << authenticator.authenticate(user2DataParser.getDataVectors()[12]) << std::endl;
}
