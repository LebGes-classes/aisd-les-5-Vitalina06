#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <random>
#include <algorithm>

// Функция для генерации случайных строк
std::string generate_random_string(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    for (size_t i = 0; i < length; ++i) {
        tmp_s += alphanum[dis(gen)];
    }

    return tmp_s;
}

// Функция для исследования производительности
void performance_test() {
    std::vector<int> sizes = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000};
    const int repetitions = 5; // Количество повторений для каждого размера
    const int string_length = 10; // Длина случайных строк

    std::cout << "Performance test results (time in microseconds):\n";
    std::cout << "Size\tEnqueue\tDequeue\tPeek\n";

    for (int size : sizes) {
        std::vector<long long> enqueue_times, dequeue_times, peek_times;

        for (int rep = 0; rep < repetitions; ++rep) {
            MaxPriorityQueue queue;
            std::vector<int> priorities(size);
            std::vector<std::string> data(size);

            // Генерация случайных данных
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 1000000);

            for (int i = 0; i < size; ++i) {
                priorities[i] = dis(gen);
                data[i] = generate_random_string(string_length);
            }

            // Тестирование Enqueue
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < size; ++i) {
                queue.enqueue(priorities[i], data[i]);
            }
            auto end = std::chrono::high_resolution_clock::now();
            enqueue_times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

            // Тестирование Peek
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; ++i) { // Многократно для более точного измерения
                queue.peek();
            }
            end = std::chrono::high_resolution_clock::now();
            peek_times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 100);

            // Тестирование Dequeue
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < size; ++i) {
                queue.dequeue();
            }
            end = std::chrono::high_resolution_clock::now();
            dequeue_times.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        // Усреднение результатов (исключаем максимальные значения)
        auto process_times = [](std::vector<long long>& times) {
            if (times.empty()) return 0LL;
            std::sort(times.begin(), times.end());
            long long sum = 0;
            for (size_t i = 0; i < times.size() - 1; ++i) {
                sum += times[i];
            }
            return sum / (times.size() - 1);
        };

        long long avg_enqueue = process_times(enqueue_times);
        long long avg_dequeue = process_times(dequeue_times);
        long long avg_peek = process_times(peek_times);

        std::cout << size << "\t" << avg_enqueue << "\t" << avg_dequeue << "\t" << avg_peek << "\n";
    }
}

int main() {
    // Основной тест
    MaxPriorityQueue queue;

    queue.enqueue(3, "Task 3");
    queue.enqueue(1, "Task 1");
    queue.enqueue(5, "Task 5");
    queue.enqueue(2, "Task 2");
    queue.enqueue(4, "Task 4");

    std::cout << "Queue size: " << queue.size() << std::endl;
    std::cout << "Top element: " << queue.peek() << std::endl;

    std::cout << "Dequeue elements in priority order:" << std::endl;
    while (!queue.isEmpty()) {
        std::cout << queue.dequeue() << std::endl;
    }

    // Запуск исследования производительности
    performance_test();

    return 0;
}