#pragma once

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition, finished;
    bool stop;
    std::atomic<int> tasks_count; // Counter for the tasks

public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template<class F>
    void enqueue(F&& f);

    void wait_until_empty();
};

ThreadPool::ThreadPool(size_t threads) : stop(false), tasks_count(0) {
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back([this] {
            for(;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                    if(this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
                tasks_count--;
                if(tasks_count == 0) finished.notify_one();
            }
        });
}

template<class F>
void ThreadPool::enqueue(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace(std::forward<F>(f));
        tasks_count++;
    }
    condition.notify_one();
}

void ThreadPool::wait_until_empty() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    finished.wait(lock, [this](){ return tasks_count == 0; });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
