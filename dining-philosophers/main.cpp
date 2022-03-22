#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

const int TOTAL = 5;

std::thread philosophers[TOTAL];
bool forks[TOTAL];
std::mutex mtx;

int counter[TOTAL];
int iterations = 0;

void philosopher(int id, int right) {

	while (iterations < 20)
	{
		std::unique_lock<std::mutex> uniq(mtx);
		if (forks[id] == true && forks[right] == true)
		{
			std::cout << id << " is EATING" << std::endl;
			counter[id]++;

			forks[id] = false;
			forks[right] = false;

			uniq.unlock();

			std::this_thread::sleep_for(std::chrono::microseconds(100));

			std::unique_lock<std::mutex> lock(mtx);

			std::cout << id << " is done eating" << std::endl;

			forks[id] = true;
			forks[right] = true;

			lock.unlock();

			std::this_thread::sleep_for(std::chrono::microseconds(100));

			iterations++;
		}
	}
}

int main() {

	for (int i = 0; i < TOTAL; i++) {
		forks[i] = true;
		counter[i] = 0;
	}

	for (int i = 0; i < TOTAL; i++) {
		if (i == 0) {
			philosophers[0] = std::thread(philosopher, 0, 4);
		}
		else {
			philosophers[i] = std::thread(philosopher, i, i - 1);
		}
	}

	for (int i = 0; i < TOTAL; i++) {
		philosophers[i].join();
	}

	std::cout << "\n";
	for (int i = 0; i < TOTAL; i++) {
		std::cout << "Philosopher_" << i + 1 << " = " << counter[i] << "\n";
	}

	return 0;
}