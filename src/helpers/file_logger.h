#pragma once

#include <string>
#include <fstream>

class FileLogger {
private:
    std::ofstream fileStream;
public:
    explicit FileLogger(const std::string &filename);

    ~FileLogger();

    void addLine(const std::string &line, bool log = true);
};

