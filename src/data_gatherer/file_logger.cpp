#include <iostream>
#include "file_logger.h"

FileLogger::FileLogger(const std::string &filename) {
    fileStream.open(filename);
}


FileLogger::~FileLogger() {
    fileStream.close();
}

void FileLogger::addLine(const std::string &line) {
    if (fileStream.is_open()) {
        fileStream << line << "\n";
        fileStream.flush();
    }
}
