all: scheduler

scheduler: main.cpp task_scheduler.cpp thread_pool.cpp
	clang++ -std=c++17 -pthread -o scheduler main.cpp task_scheduler.cpp thread_pool.cpp

clean:
	rm -f scheduler
