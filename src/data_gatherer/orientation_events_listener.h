#pragma once

#include <iostream>
#include "helpers/utils.h"
#include "lib/pstream.h"
#include "helpers/strings_helpers.h"
#include "helpers/file_logger.h"

class OrientationEventsListener {
private:
    FileLogger _orientationEventsFileLogger{"data/" + Utils::getCurrentTime() + "_orientation_events.txt"};

public:
    [[noreturn]] void listenForEvents();
};

