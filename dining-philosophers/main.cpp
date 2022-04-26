#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

/*
Привет!

Твой вариант кода вполне рабочий, присутствует лишь
1.	небольшая путаница с вилками, но это решается рефакторингом;
2.	микросекунды заменил на миллисекунды, т.к. первые – это слишком быстро для глаза человека, плохо виден сам процесс :)

Решить проблему с голоданием философов можно двумя путями:
1. как у меня в коде (это самый простой вариант); итератор «eating» у меня является локальной переменной, поэтому все философы получают одинаковое кол-во пищи;
2. использовать функцию, в которой проверять насколько конкретный философ съел больше остальных и запрещать ему брать вилки, если он, скажем, на более чем 30% съел больше остальных.

P.S. У меня 2019 студия, скорее всего, тебе нужно будет поменять в свойствах проекта версию пакета SDK и набор инструментов платформы на те, что доступны тебе в твоей версии студии.
 */

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

const int TOTAL_PHILOSOPHERS = 5;
const int EATING_MAX = 10;

std::thread philosopher_threads[TOTAL_PHILOSOPHERS];
bool forks[TOTAL_PHILOSOPHERS];
std::mutex mtx;

int feed_counter[TOTAL_PHILOSOPHERS];

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int randomize_int_in_rage(int min, int max) {
	static std::random_device rd;
	static std::mt19937 rng(rd());
	static std::uniform_int_distribution<int> distr_func(min, max);
	return distr_func(rng);
}

void philosopher_feed_job(int philosopher_id, int left_fork, int right_fork) {     // появилось дублирование, но стало чуть понятней, где какая вилка находится

	int eating = 0;
	while (eating < EATING_MAX)
	{
		{
			std::lock_guard<std::mutex> lock(mtx);                      // lock_guard с {} vs unique_lock - дело вкуса
			if (forks[left_fork] != true || forks[right_fork] != true) {
				continue;
			}
			forks[left_fork] = false;    // захватываем вилки
			forks[right_fork] = false;
			std::cout << philosopher_id << " is EATING (#" << std::this_thread::get_id() << ")" << std::endl;
		}

		int eatTime = randomize_int_in_rage(100, 500);
		std::this_thread::sleep_for(std::chrono::milliseconds(eatTime)); // даем философу время на поедание пищи

		feed_counter[philosopher_id]++;
		eating++;

		{
			std::lock_guard<std::mutex> lock(mtx);
			std::cout << philosopher_id << " is done eating" << std::endl;
			forks[left_fork] = true;    // отпускаем вилки
			forks[right_fork] = true;
		}

		int thinkingTime = randomize_int_in_rage(100, 500);
		std::this_thread::sleep_for(std::chrono::milliseconds(thinkingTime)); // философ размышляет
	}
}

int main() {

	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++) {
		forks[i] = true;
		feed_counter[i] = 0;
	}

	int left_fork;
	int right_fork;

	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++) {

		if (i == 0) {
			right_fork = TOTAL_PHILOSOPHERS - 1;
			left_fork = 0;
		}
		else {
			right_fork = i - 1;
			left_fork = i;
		}

		philosopher_threads[i] = std::thread(philosopher_feed_job, i, left_fork, right_fork);
	}

	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++) {
		philosopher_threads[i].join();
	}

	std::cout << "\n";
	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++) {
		std::cout << "Philosopher_" << i + 1 << " = " << feed_counter[i] << "\n";
	}

	return 0;
}