# File Compression & Decompression Tool (C++)

A high-performance, lossless file compression tool built in C++ using
Huffman Coding, custom bit-level encoding, block-wise file processing,
and a multi-threaded compression pipeline.

---

## 🚀 Features

- Lossless compression using Huffman Coding
- Bit-level encoding and decoding
- Block-wise processing for large files
- Multi-threaded compression pipeline
- Command-line interface
- Performance benchmarking (time, throughput, compression ratio)

---

## 🧠 Architecture

Reader Thread
|
v
Input Queue
|
v
Worker Threads (Huffman + Bit Encoding)
|
v
Output Queue
|
v
Writer Thread


---

## 🛠 Technologies Used

- **C++17**
- STL (`std::thread`, `mutex`, `condition_variable`)
- Huffman Coding
- Bit-level I/O
- CMake
- Binary File I/O

---

## 📦 Build Instructions (Windows)

### Prerequisites
- CMake
- Visual Studio Build Tools (MSVC)

```powershell
mkdir build
cd build
cmake ..
cmake --build .

▶ Usage
Compress a file
compressor compress input.txt output.huff

Decompress a file
compressor decompress output.huff restored.txt

📊 Benchmark Example
Input size       : 500 MB
Output size      : 192 MB
Compression ratio: 2.60x
Time taken       : 1.42 seconds
Throughput       : 352 MB/s
Threads used     : 8

Built a multi-threaded, block-wise, lossless file compression tool in C++ using Huffman Coding and custom bit-level encoding. Designed a producer–consumer pipeline with thread-safe queues to utilize multi-core CPUs, optimized memory and I/O performance, and benchmarked throughput and latency.

📜 License

MIT License


---

# 4️⃣ INITIALIZE GIT LOCALLY

From **project root** (`file-compressor`):

```powershell
git init
git status


You should see:

README.md
src/
CMakeLists.txt
.gitignore

Stage & commit:
git add .
git commit -m "Initial release: multithreaded Huffman compression tool"