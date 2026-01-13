#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <string>

class Decompressor
{
public:
    bool decompressFile(const std::string &inputFile,
                        const std::string &outputFile);
};

#endif
