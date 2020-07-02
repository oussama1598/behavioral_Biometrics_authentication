#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "lib/pstream.h"
#include "helpers/strings_helpers.h"

class Adb {
public:
    static void getDevices();

    static void listenForTouchEvents();
};

