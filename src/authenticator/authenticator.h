#pragma once

#include "data_parser/data_parser.h"


class Authenticator {
private:
    std::vector<double> _currentUserDataVectors;
    std::vector<double> _unknownUserDataVectors;

public:
    Authenticator(const std::string &currentUserOrientationFilename,
                  const std::string &currentUserKeyboardFilename,
                  const std::string &currentUserTouchFilename,

                  const std::string &unknownUserOrientationFilename,
                  const std::string &unknownUserKeyboardFilename,
                  const std::string &unknownUserTouchFilename);
};

