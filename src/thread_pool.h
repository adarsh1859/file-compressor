#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <functional>

class ThreadPool
{
public:
    explicit ThreadPool(size_t count);
    ~ThreadPool();

    void addTask(std::function<void()> task);
    void wait();

private:
    std::vector<std::thread> workers;
};

#endif
