#include "huffman.h"

HuffmanNode::HuffmanNode(unsigned char c, size_t f)
    : ch(c), freq(f), left(nullptr), right(nullptr) {}

HuffmanNode::HuffmanNode(HuffmanNode *l, HuffmanNode *r)
    : ch(0), freq(l->freq + r->freq), left(l), right(r) {}

struct Compare
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        return a->freq > b->freq;
    }
};

void HuffmanCoding::build(const std::vector<unsigned char> &data)
{
    std::unordered_map<unsigned char, size_t> freqMap;

    for (unsigned char c : data)
        freqMap[c]++;

    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, Compare> pq;

    for (auto &p : freqMap)
        pq.push(new HuffmanNode(p.first, p.second));

    while (pq.size() > 1)
    {
        HuffmanNode *left = pq.top();
        pq.pop();
        HuffmanNode *right = pq.top();
        pq.pop();
        pq.push(new HuffmanNode(left, right));
    }

    root = pq.top();
    generateCodes(root, "");
}

void HuffmanCoding::generateCodes(HuffmanNode *node, const std::string &prefix)
{
    if (!node)
        return;

    if (!node->left && !node->right)
    {
        codes[node->ch] = prefix.empty() ? "0" : prefix;
    }

    generateCodes(node->left, prefix + "0");
    generateCodes(node->right, prefix + "1");
}

std::unordered_map<unsigned char, std::string> HuffmanCoding::getCodes() const
{
    return codes;
}

HuffmanNode *HuffmanCoding::getRoot() const
{
    return root;
}
