#include <iostream>
#include <file.hpp>

static const size_t BUFFER_SIZE = 4096;

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

std::size_t FileReader::writeBuffer(char *buffer, size_t buffer_size)
{
    if (buffer_size < 4096)
    {
        throw std::invalid_argument("buffer size must be at least 4096 bytes.");
    }

    fileStream.read(buffer, buffer_size - 1);

    buffer[BUFFER_SIZE - 1] = EOF;

    std::streamsize bytesRead = fileStream.gcount();
    return bytesRead;
}