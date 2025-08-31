#pragma once
#include "BlockedQueue.h"
//#include "QuickSort.h"

#include <vector>
#include <thread>
#include <future>
#include <memory>
#include <functional>

//void taskFunc(int id, int delay);
// удобное определение для сокращения кода
typedef std::function<void()> task_type;
// тип указатель на функцию, которая является эталоном для функций задач
typedef void (*FuncType) (long*, long, long,
    std::shared_ptr<std::promise<void>>);
// пул потоков
class ThreadPool
{
public:
    ThreadPool();
    
    // запуск:
    void start();
    // остановка:
    void stop();
    // проброс задач    
    void push_task(FuncType f, long* array, long left, long right,
        std::shared_ptr<std::promise<void>>);
    // функция входа для потока
    void threadFunc(int qindex);
    
private:
    // количество потоков
    int m_thread_count;
    // потоки
    std::vector<std::thread> m_threads;
    // очереди задач для потоков
    std::vector<BlockedQueue<task_type>> m_thread_queues;
    // для равномерного распределения задач
    int m_index{ 0 };
};

