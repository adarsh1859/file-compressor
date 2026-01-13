#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue
{
public:
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(m);
        q.push(std::move(item));
        cv.notify_one();
    }

    bool pop(T &item)
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&]
                { return !q.empty() || finished; });

        if (q.empty())
            return false;

        item = std::move(q.front());
        q.pop();
        return true;
    }

    void setFinished()
    {
        std::unique_lock<std::mutex> lock(m);
        finished = true;
        cv.notify_all();
    }

private:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
    bool finished = false;
};

#endif
