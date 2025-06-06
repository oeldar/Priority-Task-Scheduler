#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <stdexcept>

class ThreadPool {
  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop_flag{false};

  public:
    explicit ThreadPool(size_t num_threads);

    template<typename F>
      void enqueue(F&& f);

    ~ThreadPool();
};

template<typename F>
void ThreadPool::enqueue(F&& f) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (stop_flag) {
      throw std::runtime_error("ThreadPool je zaustavljen.");
    }
    task_queue.emplace(std::forward<F>(f));
  }
  condition.notify_one();
}
