#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class FileReader
{
private:
    std::string filePath;
    std::ifstream fileStream;

public:
    FileReader(const std::string &filePath) : filePath(filePath), fileStream() {}
    FileReader() = default;
    ~FileReader();
    void close() { fileStream.close(); }
    bool isEndOfFile() const { return fileStream.eof(); }
    std::size_t writeBuffer(char *buffer, size_t length);
    FileReader &open();
};