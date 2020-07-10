#include <iostream>
#include "file_logger.h"

FileLogger::FileLogger(const std::string &filename) {
    fileStream.open(filename);
}


FileLogger::~FileLogger() {
    fileStream.close();
}

void FileLogger::addLine(const std::string &line, bool log) {
    if (fileStream.is_open()) {
        if (log) std::cout << line << std::endl;

        fileStream << line << "\n";
        fileStream.flush();
    }
}
