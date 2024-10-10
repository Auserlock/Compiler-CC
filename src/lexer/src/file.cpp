#include <iostream>
#include "file.hpp"

FileReader &FileReader::open()
{
    fileStream.open(filePath);
    if (!fileStream.is_open())
    {
        throw std::runtime_error("Failed to open the file: " + filePath);
    }
    return *this;
}

FileReader::~FileReader()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

std::size_t FileReader::readBuffer(char *buffer, std::size_t bufferSize)
{
    if (bufferSize < 4096)
    {
        throw std::invalid_argument("Buffer size must be at least 4096 bytes.");
    }

    fileStream.read(buffer, bufferSize - 1);
    std::streamsize bytesRead = fileStream.gcount();

    buffer[4095] = EOF;
    return bytesRead;
}