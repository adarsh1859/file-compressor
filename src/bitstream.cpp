#include "bitstream.h"

// ================= BitWriter =================

BitWriter::BitWriter(std::ofstream &output)
    : outFile(&output), buffer(0), bitCount(0) {}

BitWriter::BitWriter(std::vector<char> &bufferVec)
    : outBuffer(&bufferVec), buffer(0), bitCount(0) {}

void BitWriter::writeByte(uint8_t byte)
{
    if (outFile)
        outFile->put(static_cast<char>(byte));
    else
        outBuffer->push_back(static_cast<char>(byte));
}

void BitWriter::writeBit(bool bit)
{
    buffer = (buffer << 1) | (bit ? 1 : 0);
    bitCount++;

    if (bitCount == 8)
    {
        writeByte(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::writeBits(const std::string &bits)
{
    for (char c : bits)
        writeBit(c == '1');
}

void BitWriter::flush()
{
    if (bitCount > 0)
    {
        buffer <<= (8 - bitCount);
        writeByte(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

// ================= BitReader =================

BitReader::BitReader(std::ifstream &input)
    : inFile(&input), buffer(0), bitCount(0) {}

BitReader::BitReader(const std::vector<char> &bufferVec)
    : inBuffer(&bufferVec), buffer(0), bitCount(0), bufferPos(0) {}

bool BitReader::readBit(bool &bit)
{
    if (bitCount == 0)
    {
        if (inFile)
        {
            if (!inFile->get(reinterpret_cast<char &>(buffer)))
                return false;
        }
        else
        {
            if (bufferPos >= inBuffer->size())
                return false;
            buffer = static_cast<uint8_t>((*inBuffer)[bufferPos++]);
        }
        bitCount = 8;
    }

    bit = (buffer & 0x80) != 0;
    buffer <<= 1;
    bitCount--;
    return true;
}
