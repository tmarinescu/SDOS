#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "main.hpp"

class Mutex
{
private:
	volatile uint32_t _semaphore;
	bool _testAndSet(uint32_t comp, uint32_t set) volatile;
public:
	Mutex();
	volatile bool Locked;
	void Lock() volatile;
	void Unlock() volatile;
};

class AutoMutex
{
public:
	Mutex* mainMutex = 0;
	AutoMutex(Mutex* mtx);
	~AutoMutex();
};

#endif