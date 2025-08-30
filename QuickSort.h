#pragma once
#include <thread>
#include <mutex>

class Array
{
public:
	Array() {}
	~Array() {}

	void start();

private:
	//int* array_;
	long numberOfElements_{ 0 };
	size_t numberOfCores_ = std::thread::hardware_concurrency();
	std::mutex mutex_;
	bool isThreadSorting_ = true;

	void selectNumberOfElements();
	void createArray(long* array);
	void copyingAReferenceArray(long* array_, long* array);
	void swapElements(long& first, long& second);
	void quickSort(long* array, long left, long right);
	bool checkSorting(long* array);
	void printResult(double sortingTime);
};

