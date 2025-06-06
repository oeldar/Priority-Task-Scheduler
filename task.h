#pragma once
#include <chrono>

struct Task {
  int id;
  std::chrono::milliseconds start_time;
  std::chrono::milliseconds duration;
  int priority;

  Task(int id, int start_ms, int duration_ms, int priority)
    : id(id),
      start_time(std::chrono::milliseconds(start_ms)),
      duration(std::chrono::milliseconds(duration_ms)),
      priority(priority) {}

  bool operator<(const Task& other) const {
    if (start_time == other.start_time) {
      return priority > other.priority;
    }
    return start_time > other.start_time;
  }
};
