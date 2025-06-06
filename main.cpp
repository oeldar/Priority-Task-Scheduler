#include "task.h"
#include "task_scheduler.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  size_t num_threads = 3;

  if (argc > 2) {
    std::cerr << "Greska: Previse argumenata." << std::endl;
    std::cerr << "Upotreba:  " << argv[0] << " [broj_niti]" << std::endl;
    return 1;
  }

  if (argc == 2) {
    try {
      int temp = std::stoi(argv[1]);
      if (temp < 0) {
        std::cerr << "Greska: Broj niti mora biti pozitivan." << std::endl;
        return 1;
      }
      num_threads = static_cast<size_t>(temp);
    } catch (const std::exception &e) {
      std::cerr << "Greska: Neispravan broj niti." << std::endl;
      std::cerr << "Upotreba: " << argv[0] << "[broj_niti]" << std::endl;
      return 1;
    }
  } else {
    std::cout << "Napomena: Mozete koristiti " << argv[0]
              << " [broj_niti] za specifikaciju broja radnih niti."
              << std::endl;
    std::cout << std::endl;
  }

  std::cout << "=== TASK SCHEDULER ===" << std::endl;
  std::cout << "Inicijalizacija scheduler-a sa " << num_threads
            << " radnih niti..." << std::endl;

  auto start_execution = std::chrono::steady_clock::now();

  {
    TaskScheduler scheduler(num_threads);

    std::vector<Task> tasks = {Task(1, 1000, 3000, 2), Task(2, 1000, 1000, 1),
                               Task(3, 1500, 2000, 3), Task(4, 1500, 1000, 1)};

    std::cout << "\nDodavanje zadataka:" << std::endl;
    for (const auto &task : tasks) {
      std::cout << "- Zadatak " << task.id
                << ": start=" << task.start_time.count() << "ms"
                << ", duration=" << task.duration.count() << "ms"
                << ", priority=" << task.priority << std::endl;
      scheduler.schedule_task(task);
    }

    std::cout << "\n=== IZVRSAVANJE ZADATAKA ===" << std::endl;
    std::cout << "Ocekivani redoslijed: 2 -> 1 -> 4 -> 3" << std::endl;
    std::cout << "Pracenje izvrsavanja:\n" << std::endl;

    scheduler.wait_for_completion();
  }

  auto end_execution = std::chrono::steady_clock::now();
  auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_execution - start_execution);

  std::cout << "\n=== STATISTIKE IZVRSAVANJA ===" << std::endl;
  std::cout << "Ukupno vrijeme izvrsavanja: " << total_duration.count() << "ms"
            << std::endl;
  std::cout << "Broj koristenih radnih niti: " << num_threads << std::endl;

  return 0;
}
