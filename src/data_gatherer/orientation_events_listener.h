#pragma once

#include <iostream>
#include "lib/pstream.h"
#include "helpers/strings_helpers.h"

class OrientationEventsListener {
public:
    [[noreturn]] void listenForEvents();
};

