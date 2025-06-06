#include "task_scheduler.h"
#include <algorithm>
#include <iostream>

TaskScheduler::TaskScheduler(size_t num_threads)
    : thread_pool(num_threads), start_time(std::chrono::steady_clock::now()) {
  running = true;
  scheduler_thread = std::thread(&TaskScheduler::scheduler_loop, this);
}

void TaskScheduler::scheduler_loop() {
  while (running) {
    std::unique_lock<std::mutex> lock(scheduler_mutex);

    scheduler_cv.wait_for(lock, std::chrono::milliseconds(10), [this] {
      return !running || (!scheduled_tasks.empty() &&
                          should_execute_task(scheduled_tasks.top()));
    });

    if (!running)
      break;

    std::vector<Task> ready_tasks;
    auto current_time = get_current_time();

    while (!scheduled_tasks.empty() &&
           scheduled_tasks.top().start_time <= current_time) {
      ready_tasks.push_back(scheduled_tasks.top());
      scheduled_tasks.pop();
    }

    std::sort(ready_tasks.begin(), ready_tasks.end(),
              [](const Task &a, const Task &b) {
                if (a.start_time == b.start_time) {
                  return a.priority < b.priority;
                }
                return a.start_time < b.start_time;
              });

    for (const auto &task : ready_tasks) {
      thread_pool.enqueue([task, this]() { execute_task(task); });
    }
  }
}

bool TaskScheduler::should_execute_task(const Task &task) const {
  return task.start_time <= get_current_time();
}

std::chrono::milliseconds TaskScheduler::get_current_time() const {
  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
  return elapsed;
}

void TaskScheduler::execute_task(const Task &task) {
  auto actual_start = get_current_time();

  {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "[" << actual_start.count() << "ms] " << "Zadatak " << task.id
              << " POCETAK (prioritet: " << task.priority
              << ", planirano: " << task.start_time.count() << "ms)"
              << " [Thread: " << std::this_thread::get_id() << "]" << std::endl;
  }

  std::this_thread::sleep_for(task.duration);

  auto actual_end = get_current_time();

  {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "[" << actual_end.count() << "ms] " << "Zadatak " << task.id
              << " ZAVRSEN"
              << " (trajanje: " << (actual_end - actual_start).count() << "ms)"
              << " [Thread: " << std::this_thread::get_id() << "]" << std::endl;
  }
}

void TaskScheduler::schedule_task(const Task &task) {
  std::lock_guard<std::mutex> lock(scheduler_mutex);
  scheduled_tasks.push(task);
  scheduler_cv.notify_one();
}

void TaskScheduler::wait_for_completion() {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(scheduler_mutex);
      if (scheduled_tasks.empty())
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


TaskScheduler::~TaskScheduler() {
  running = false;
  scheduler_cv.notify_all();

  if (scheduler_thread.joinable())
    scheduler_thread.join();
}
