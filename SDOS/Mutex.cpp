#include "Mutex.hpp"

Mutex::Mutex()
{
	__DMB();
	_semaphore = 0;
}

bool Mutex::_testAndSet(uint32_t new_val, uint32_t old_val) volatile
{
	do
	{
		if (__LDREXW(&_semaphore) != old_val) //something else changed it
			return false;
	} while (__STREXW(new_val, &_semaphore) != 0); //lock while attempting to set it
	__DMB();    //data memory barrier
	return true;
}

void Mutex::Lock() volatile
{
	while (true)
	{
		if (_semaphore == 0)
		{
			if (_testAndSet(1, 0))
			{
				Locked = true;
				return;
			}
		}
		//Yield();
	}
}

void Mutex::Unlock() volatile
{
	__DMB();
	_semaphore = 0;
	Locked = false;
}

AutoMutex::AutoMutex(Mutex* mtx)
{
	mainMutex = mtx;
	mtx->Lock();
}
	
AutoMutex::~AutoMutex()
{
	mainMutex->Unlock();
}