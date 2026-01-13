#include "thread_pool.h"

ThreadPool::ThreadPool(size_t count)
{
    workers.reserve(count);
}

ThreadPool::~ThreadPool()
{
    wait();
}

void ThreadPool::addTask(std::function<void()> task)
{
    workers.emplace_back(task);
}

void ThreadPool::wait()
{
    for (auto &t : workers)
    {
        if (t.joinable())
            t.join();
    }
    workers.clear();
}
