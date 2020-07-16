#include "authenticator.h"

int main() {
    Authenticator authenticator{
            // current user data
            "data/oussama_orientation_events.txt",
            "data/oussama_keyboard_events.txt",
            "data/oussama_touch_events.txt",

            // unknown user data
            "data/ibtissam_orientation_events.txt",
            "data/ibtissam_keyboard_events.txt",
            "data/ibtissam_touch_events.txt"
    };

    authenticator.train();

//    std::cout << authenticator.authenticate({}) << std::endl;
}
