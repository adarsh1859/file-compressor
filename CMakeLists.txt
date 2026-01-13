cmake_minimum_required(VERSION 3.10)
project(FileCompressor)

set(CMAKE_CXX_STANDARD 17)

add_executable(compressor
    src/main.cpp
    src/huffman.cpp
    src/bitstream.cpp
    src/compressor.cpp
    src/decompressor.cpp
    src/thread_pool.cpp
)
