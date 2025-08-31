#include "QuickSort.h"

#include <iostream>
//#include <thread>
#include <future>
#include <random>
#include <chrono>

Array::Array()
{
    threadPool_.start();
}

Array::~Array()
{
    threadPool_.stop();
}

void Array::start()
{
    // We use one set of elements for a pure result.
    selectNumberOfElements();
    long* array_ = new long[numberOfElements_];
    long* array = new long[numberOfElements_];
    createArray(array_);
    copyingAReferenceArray(array_, array);

    std::cout << "CPU cores detected: " << numberOfCores_ << "\n";
    numberOfCores_ = std::thread::hardware_concurrency() != 0
        ? std::thread::hardware_concurrency() : 4;
    std::cout << "Number of the threads: " << numberOfCores_ << "\n";
    std::cout << "Number of elements : " << numberOfElements_ << "\n";

    // многопоточный запуск
    std::cout << "Start asynchronous sorting...\n";
    auto start = std::chrono::high_resolution_clock::now();
    counter_ptr_ = std::make_shared<std::promise<void>>();
    auto endSorting = counter_ptr_->get_future();
    quickSortThread(array_, 0, numberOfElements_ - 1, counter_ptr_);
    endSorting.wait();
    auto finish = std::chrono::high_resolution_clock::now();
    double sortingTime = static_cast<double>(
        (std::chrono::duration<double>(finish - start)).count());

    if (checkSorting(array_))
    {
        printResult(sortingTime);
    }
    delete[] array_;

    // однопоточный запуск
    isMultithreadedSorting_ = false;
    std::cout << "Start synchronous sorting...\n";
    start = std::chrono::high_resolution_clock::now();
    quickSort(array, 0, numberOfElements_ - 1);
    finish = std::chrono::high_resolution_clock::now();
    sortingTime = static_cast<double>(
        (std::chrono::duration<double>(finish - start)).count());

    if (checkSorting(array))
    {
        printResult(sortingTime);
    }
    delete[] array;
}

void Array::selectNumberOfElements()
{
    std::cout <<
        "Select the number of elements in the array (recommend 10000000): ";
    std::cin >> numberOfElements_;
    if (numberOfElements_ < 1'000)
    {
        numberOfElements_ = 10'000'000;
    }
}

void Array::createArray(long* array)
{
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0,
        numberOfElements_);

    for (long i{ 0 }; i < numberOfElements_; i++)
        array[i] = distribution(generator);
}

void Array::copyingAReferenceArray(long* array_, long* array)
{
    for (long element{ 0 }; element < numberOfElements_; element++)
    {
        array[element] = array_[element];
    }
}

void Array::swapElements(long& first, long& second)
{
    int temp = first;
    first = second;
    second = temp;
}

void Array::quickSortThread(long* array, long left, long right,
    std::shared_ptr<std::promise<void>> counter_ptr)
{
    if (left >= right)
    {        
        std::lock_guard<std::mutex> locker(countLock_);
        if (counter_ptr_.use_count() <= 2)
        {
            counter_ptr_->set_value();
        }
        return;
    }

    long left_bound = left;
    long right_bound = right;

    reallocationOfElements(array, left_bound, right_bound);

    if (isMultithreadedSorting_ && (right_bound - left > 10000))
    {
        // если элементов в левой части больше чем 10000
        // вызываем асинхронно рекурсию для правой части
        threadPool_.push_task(&Array::quickSortThread, std::ref(array),
            left, right, std::make_shared<std::promise<void>>(counter_ptr));
        /*auto f = async(std::launch::async, [&]() {
            quickSort(array, left, right_bound);
            });*/
        quickSort(array, left_bound, right);
    }
    else {
        // запускаем обе части синхронно
        quickSort(array, left, right_bound);
        quickSort(array, left_bound, right);
    }
}

void Array::quickSort(long* array, long left, long right)
{
    if (left >= right)
    {
        return;
    }

    long left_bound = left;
    long right_bound = right;

    reallocationOfElements(array, left_bound, right_bound);

    quickSort(array, left, right_bound);
    quickSort(array, left_bound, right);    
}

void Array::reallocationOfElements(long* array, long& left_bound, long& right_bound)
{
    long middle = array[(left_bound + right_bound) / 2];

    do {
        while (array[left_bound] < middle)
        {
            left_bound++;
        }
        while (array[right_bound] > middle)
        {
            right_bound--;
        }

        //Меняем элементы местами
        if (left_bound <= right_bound)
        {
            swapElements(array[left_bound], array[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);
}

bool Array::checkSorting(long* array)
{
    for (long element{ 0 }; element < numberOfElements_ - 1; element++)
    {
        if (array[element] > array[element + 1])
        {
            std::cout << "Unsorted\n";
            return false;
        }
    }
    return true;
}

void Array::printResult(double sortingTime)
{
    std::cout << (isMultithreadedSorting_
        ? "Asynchronous sorting time: " : "Synchronous sorting time: ")
        << sortingTime << " seconds\n";
}
