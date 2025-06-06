#pragma once
#include "thread_pool.h"
#include "task.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

class TaskScheduler {
  private:
    ThreadPool thread_pool;
    std::priority_queue<Task> scheduled_tasks;
    std::mutex scheduler_mutex;
    std::condition_variable scheduler_cv;
    std::atomic<bool> running{false};
    std::thread scheduler_thread;
    std::chrono::steady_clock::time_point start_time;
    std::mutex print_mutex;

    void scheduler_loop();
    bool should_execute_task(const Task& task) const;
    std::chrono::milliseconds get_current_time() const;
    void execute_task(const Task& task);

  public:
    explicit TaskScheduler(size_t num_threads);
    void schedule_task(const Task& task);
    void wait_for_completion();
    ~TaskScheduler();
};
