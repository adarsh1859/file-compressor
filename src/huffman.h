#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

struct HuffmanNode
{
    unsigned char ch;
    size_t freq;
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(unsigned char c, size_t f);
    HuffmanNode(HuffmanNode *l, HuffmanNode *r);
};

class HuffmanCoding
{
public:
    void build(const std::vector<unsigned char> &data);
    std::unordered_map<unsigned char, std::string> getCodes() const;
    HuffmanNode *getRoot() const;

private:
    HuffmanNode *root = nullptr;
    std::unordered_map<unsigned char, std::string> codes;

    void generateCodes(HuffmanNode *node, const std::string &prefix);
};

#endif
