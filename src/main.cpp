#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>

#include "compressor.h"
#include "decompressor.h"

using Clock = std::chrono::high_resolution_clock;

void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "  compressor compress <input_file> <output_file>\n";
    std::cout << "  compressor decompress <input_file> <output_file>\n";
}

uint64_t fileSize(const std::string &path)
{
    return std::filesystem::file_size(path);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    auto start = Clock::now();

    if (mode == "compress")
    {
        Compressor compressor;
        if (!compressor.compressFile(inputFile, outputFile))
        {
            std::cerr << "Compression failed\n";
            return 1;
        }

        auto end = Clock::now();
        double seconds =
            std::chrono::duration<double>(end - start).count();

        uint64_t inSize = fileSize(inputFile);
        uint64_t outSize = fileSize(outputFile);

        std::cout << "Compression successful\n";
        std::cout << "Input size       : " << inSize << " bytes\n";
        std::cout << "Output size      : " << outSize << " bytes\n";
        std::cout << "Compression ratio: "
                  << static_cast<double>(inSize) / outSize << "x\n";
        std::cout << "Time taken       : " << seconds << " seconds\n";
        std::cout << "Throughput       : "
                  << (inSize / (1024.0 * 1024.0)) / seconds
                  << " MB/s\n";
    }
    else if (mode == "decompress")
    {
        Decompressor decompressor;
        if (!decompressor.decompressFile(inputFile, outputFile))
        {
            std::cerr << "Decompression failed\n";
            return 1;
        }

        auto end = Clock::now();
        double seconds =
            std::chrono::duration<double>(end - start).count();

        std::cout << "Decompression successful\n";
        std::cout << "Time taken: " << seconds << " seconds\n";
    }
    else
    {
        printUsage();
        return 1;
    }

    return 0;
}
