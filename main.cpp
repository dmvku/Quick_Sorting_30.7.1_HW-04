#include "RequestHandler.h"

#include <iostream>
#include <mutex>

std::mutex coutLocker;

// функци€, выполн€юща€ задачу
void taskFunc(int id, int delay) {
    // имитируем врем€ выполнени€ задачи
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    // выводим информацию о завершении
    std::unique_lock<std::mutex> l(coutLocker);
    std::cout << "task " << id << " made by thread_id "
        << std::this_thread::get_id() << "\n";
}

int main() {
    srand(0);
    
    ThreadPool pool;
    pool.start();
    for (int i = 0; i < 20; i++)
    {
        pool.push_task(taskFunc, i, 1 + rand() % 4);
    }
    pool.stop();
    return 0;
}