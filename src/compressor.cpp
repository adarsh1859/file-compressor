#include "compressor.h"
#include "huffman.h"
#include "bitstream.h"
#include "safe_queue.h"
#include "thread_pool.h"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <cstring>
#include <cstdint>

static const uint32_t BLOCK_SIZE = 4 * 1024 * 1024; // 4 MB

// -------------------- Data Structures --------------------

struct Block
{
    uint32_t id;
    std::vector<unsigned char> data;
};

struct CompressedBlock
{
    uint32_t id;
    std::vector<char> buffer;
};

// -------------------- Compressor --------------------

bool Compressor::compressFile(const std::string &inputFile,
                              const std::string &outputFile)
{
    std::ifstream in(inputFile, std::ios::binary);
    if (!in)
        return false;

    std::ofstream out(outputFile, std::ios::binary);
    if (!out)
        return false;

    // -------- Global Header --------
    out.write("HUFF", 4);
    out.write(reinterpret_cast<const char *>(&BLOCK_SIZE), sizeof(BLOCK_SIZE));

    SafeQueue<Block> inputQueue;
    SafeQueue<CompressedBlock> outputQueue;

    std::atomic<uint32_t> blockCounter{0};

    // ==================== Reader Thread ====================
    std::thread reader([&]()
                       {
        while (true) {
            Block block;
            block.id = blockCounter++;
            block.data.resize(BLOCK_SIZE);

            in.read(reinterpret_cast<char*>(block.data.data()), BLOCK_SIZE);
            std::streamsize bytesRead = in.gcount();

            if (bytesRead <= 0)
                break;

            block.data.resize(static_cast<size_t>(bytesRead));
            inputQueue.push(std::move(block));
        }
        inputQueue.setFinished(); });

    // ==================== Worker Threads ====================
    size_t workerCount = std::thread::hardware_concurrency();
    if (workerCount == 0)
        workerCount = 4;

    ThreadPool pool(workerCount);

    for (size_t i = 0; i < workerCount; ++i)
    {
        pool.addTask([&]()
                     {
            Block block;
            while (inputQueue.pop(block)) {

                // ----- Build Huffman Tree -----
                HuffmanCoding hc;
                hc.build(block.data);
                auto codes = hc.getCodes();

                // ----- Frequency Table -----
                std::unordered_map<unsigned char, uint32_t> freq;
                for (unsigned char c : block.data)
                    freq[c]++;

                // ----- Block Buffer -----
                std::vector<char> buffer;

                uint32_t originalSize =
                    static_cast<uint32_t>(block.data.size());
                uint16_t symbolCount =
                    static_cast<uint16_t>(freq.size());

                // original size
                buffer.insert(buffer.end(),
                    reinterpret_cast<char*>(&originalSize),
                    reinterpret_cast<char*>(&originalSize) + sizeof(originalSize));

                // symbol count
                buffer.insert(buffer.end(),
                    reinterpret_cast<char*>(&symbolCount),
                    reinterpret_cast<char*>(&symbolCount) + sizeof(symbolCount));

                // frequency table
                for (auto& p : freq) {
                    buffer.push_back(static_cast<char>(p.first));
                    buffer.insert(buffer.end(),
                        reinterpret_cast<char*>(&p.second),
                        reinterpret_cast<char*>(&p.second) + sizeof(p.second));
                }

                // placeholder for compressed size
                size_t compressedSizePos = buffer.size();
                uint32_t compressedSize = 0;
                buffer.resize(buffer.size() + sizeof(uint32_t));

                // ----- Compress Data (IN MEMORY) -----
                std::vector<char> compressedData;
                BitWriter writer(compressedData);

                for (unsigned char c : block.data)
                    writer.writeBits(codes[c]);

                writer.flush();

                compressedSize =
                    static_cast<uint32_t>(compressedData.size());

                // patch compressed size
                std::memcpy(buffer.data() + compressedSizePos,
                            &compressedSize,
                            sizeof(uint32_t));

                // append compressed bytes
                buffer.insert(buffer.end(),
                              compressedData.begin(),
                              compressedData.end());

                outputQueue.push({ block.id, std::move(buffer) });
            } });
    }

    // ==================== Writer Thread ====================
    std::thread writer([&]()
                       {
        CompressedBlock cb;
        while (outputQueue.pop(cb)) {
            out.write(cb.buffer.data(), cb.buffer.size());
        } });

    // ==================== Shutdown ====================
    reader.join();
    pool.wait();
    outputQueue.setFinished();
    writer.join();

    return true;
}
