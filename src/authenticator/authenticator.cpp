#include "authenticator.h"

Authenticator::Authenticator(const std::string &currentUserOrientationFilename,
                             const std::string &currentUserKeyboardFilename,
                             const std::string &currentUserTouchFilename,

                             const std::string &unknownUserOrientationFilename,
                             const std::string &unknownUserKeyboardFilename,
                             const std::string &unknownUserTouchFilename) {
    DataParser currentUserDataParser;
    DataParser unknownUserDataParser;

    currentUserDataParser.combineLogs(currentUserOrientationFilename, currentUserKeyboardFilename,
                                      currentUserTouchFilename);
    unknownUserDataParser.combineLogs(unknownUserOrientationFilename, unknownUserKeyboardFilename,
                                      unknownUserTouchFilename);

    currentUserDataParser.parseDataSlices();
    unknownUserDataParser.parseDataSlices();


    currentUserDataParser.getDataVectors();
}
