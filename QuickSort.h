#pragma once
#include "ThreadPool.h"

//#include <thread>
#include <mutex>
#include <memory>

class Array
{
public:
	Array();
	~Array();

	void start();

private:	
	ThreadPool threadPool_;	
	std::shared_ptr<std::promise<void>> counter_ptr_ = nullptr;
	std::mutex countLock_;

	long numberOfElements_{ 0 };
	size_t numberOfCores_ = std::thread::hardware_concurrency();
	//std::mutex mutex_;
	bool isMultithreadedSorting_ = true;

	void selectNumberOfElements();
	void createArray(long* array);
	void copyingAReferenceArray(long* array_, long* array);
	void swapElements(long& first, long& second);
	void quickSortThread(long* array, long left, long right,
		std::shared_ptr<std::promise<void>> counter_ptr);
	void quickSort(long* array, long left, long right);
	void reallocationOfElements(long* array, long& left, long& right);
	bool checkSorting(long* array);
	void printResult(double sortingTime);
};

