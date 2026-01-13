#include "decompressor.h"
#include "huffman.h"
#include "bitstream.h"

#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdint>

bool Decompressor::decompressFile(const std::string &inputFile,
                                  const std::string &outputFile)
{
    std::ifstream in(inputFile, std::ios::binary);
    if (!in)
        return false;

    std::ofstream out(outputFile, std::ios::binary);
    if (!out)
        return false;

    // ---- Global header ----
    char magic[4];
    in.read(magic, 4);
    if (std::string(magic, 4) != "HUFF")
        return false;

    uint32_t blockSize;
    in.read(reinterpret_cast<char *>(&blockSize), sizeof(blockSize));

    // ---- Process blocks ----
    while (true)
    {
        uint32_t originalSize;
        uint16_t symbolCount;

        in.read(reinterpret_cast<char *>(&originalSize), sizeof(originalSize));
        if (!in)
            break;

        in.read(reinterpret_cast<char *>(&symbolCount), sizeof(symbolCount));

        std::unordered_map<unsigned char, uint32_t> freq;
        for (uint16_t i = 0; i < symbolCount; ++i)
        {
            unsigned char ch;
            uint32_t f;
            in.get(reinterpret_cast<char &>(ch));
            in.read(reinterpret_cast<char *>(&f), sizeof(f));
            freq[ch] = f;
        }

        uint32_t compressedSize;
        in.read(reinterpret_cast<char *>(&compressedSize), sizeof(compressedSize));

        std::vector<char> compressedData(compressedSize);
        in.read(compressedData.data(), compressedSize);

        // ---- Rebuild Huffman tree ----
        std::vector<unsigned char> dummy;
        for (auto &p : freq)
            dummy.insert(dummy.end(), p.second, p.first);

        HuffmanCoding hc;
        hc.build(dummy);
        HuffmanNode *root = hc.getRoot();

        // ---- Decode bits from MEMORY ----
        BitReader reader(compressedData);
        HuffmanNode *curr = root;
        bool bit;
        uint32_t decoded = 0;

        while (decoded < originalSize && reader.readBit(bit))
        {
            curr = bit ? curr->right : curr->left;

            if (!curr->left && !curr->right)
            {
                out.put(static_cast<char>(curr->ch));
                curr = root;
                decoded++;
            }
        }
    }

    return true;
}
