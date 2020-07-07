#pragma once

#include <iostream>
#include "helpers/utils.h"
#include "lib/pstream.h"
#include "helpers/strings_helpers.h"

class OrientationEventsListener {
public:
    [[noreturn]] void listenForEvents();
};

