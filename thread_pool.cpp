#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) {
  for (size_t i = 0; i < num_threads; ++i) {
    workers.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(queue_mutex);
          condition.wait(lock,
                         [this] { return stop_flag || !task_queue.empty(); });

          if (stop_flag && task_queue.empty())
            return;

          task = std::move(task_queue.front());
          task_queue.pop();
        }
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop_flag = true;
  }

  condition.notify_all();

  for (std::thread& worker : workers) {
    if (worker.joinable())
      worker.join();
  }

}
