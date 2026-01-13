#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

class BitWriter
{
public:
    // File-based
    explicit BitWriter(std::ofstream &output);

    // Memory-based
    explicit BitWriter(std::vector<char> &buffer);

    void writeBit(bool bit);
    void writeBits(const std::string &bits);
    void flush();

private:
    std::ofstream *outFile = nullptr;
    std::vector<char> *outBuffer = nullptr;

    uint8_t buffer;
    int bitCount;

    void writeByte(uint8_t byte);
};

class BitReader
{
public:
    explicit BitReader(std::ifstream &input);
    explicit BitReader(const std::vector<char> &buffer);

    bool readBit(bool &bit);

private:
    std::ifstream *inFile = nullptr;
    const std::vector<char> *inBuffer = nullptr;
    size_t bufferPos = 0;

    uint8_t buffer;
    int bitCount;
};

#endif
