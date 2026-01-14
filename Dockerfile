# -------- Stage 1: Build --------
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make

WORKDIR /app

# Copy source code
COPY . .

# Build project
RUN rm -rf build && mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .

# -------- Stage 2: Runtime --------
FROM ubuntu:22.04

WORKDIR /app

# Copy the final binary (Linux single-config path)
COPY --from=builder /app/build/compressor /app/compressor

ENTRYPOINT ["./compressor"]
