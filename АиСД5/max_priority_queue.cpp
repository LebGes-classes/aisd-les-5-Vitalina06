#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class MaxPriorityQueue {
private:
    struct Node {
        int priority;
        std::string data;
        
        Node(int p, const std::string& d) : priority(p), data(d) {}
    };

    std::vector<Node> heap;

    // Вспомогательные функции для работы с кучей
    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].priority >= heap[index].priority) {
                break;
            }
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void siftDown(int index) {
        int left, right, largest;
        int size = heap.size();
        
        while (true) {
            left = 2 * index + 1;
            right = 2 * index + 2;
            largest = index;

            if (left < size && heap[left].priority > heap[largest].priority) {
                largest = left;
            }
            if (right < size && heap[right].priority > heap[largest].priority) {
                largest = right;
            }
            if (largest == index) {
                break;
            }
            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    // Добавление элемента в очередь
    void enqueue(int priority, const std::string& data) {
        heap.emplace_back(priority, data);
        siftUp(heap.size() - 1);
    }

    // Извлечение элемента с наивысшим приоритетом
    std::string dequeue() {
        if (heap.empty()) {
            throw std::out_of_range("Queue is empty");
        }
        
        std::string result = heap[0].data;
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            siftDown(0);
        }
        
        return result;
    }

    // Просмотр элемента с наивысшим приоритетом без извлечения
    std::string peek() const {
        if (heap.empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return heap[0].data;
    }

    // Проверка на пустоту
    bool isEmpty() const {
        return heap.empty();
    }

    // Получение размера очереди
    size_t size() const {
        return heap.size();
    }
};

int main() {
    MaxPriorityQueue queue;

    // Тестирование очереди
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

    return 0;
}