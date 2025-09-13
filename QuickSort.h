#pragma once
#include "ThreadPool.h"

#include <memory>
#include <future>
#include <atomic>

class Array
{
public:
	Array();
	~Array();

	void start();

private:	
	ThreadPool threadPool_;	
	std::shared_ptr<std::promise<void>> counter_ptr_ = nullptr;	
	std::atomic<long> taskCounter_;

	long numberOfElements_{ 0 };
	size_t numberOfCores_ = std::thread::hardware_concurrency();	
	bool isMultithreadedSorting_ = true;

	void selectNumberOfElements();
	void createArray(long* array);
	void copyingAReferenceArray(long* array_, long* array);	
	void quickSortThread(long* array, long left, long right,
		std::shared_ptr<std::promise<void>> task);
	void functionTotransferToThePool(long* array, long left, long right);
	void quickSort(long* array, long left, long right);
	void reallocationOfElements(long* array, long& left, long& right);
	void swapElements(long& first, long& second);
	bool checkSorting(long* array);
	void printResult(double sortingTime);
};

