#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

/**
 * This ThreadsafeQueue is from the C++ Concurrency in Action book
 * with some small modifications.
 */

template<typename T>
class ThreadsafeQueue
{
private:
    // mutable keyword allows modification of the mutex value.
    mutable std::mutex mut;
    // stdlib queue - no reason to reinvent the wheel!
    std::queue<T> data_queue;
    // Sets up signaling to ensure correct pushing/popping on the queue.
    std::condition_variable data_cond;
public:
    
    ThreadsafeQueue() {}
    
    void push(T new_value)
    {
        std::lock_guard lk { mut };
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock lk { mut };
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock lk { mut };
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard lk { mut };
        if(data_queue.empty())
            return false;
        value=std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard lk { mut };
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard lk { mut };
        return data_queue.empty();
    }
};

#endif // THREADSAFE_QUEUE_HPP