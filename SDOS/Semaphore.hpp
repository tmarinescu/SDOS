#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "main.hpp"
#include "Critical.hpp"

class Semaphore
{
public:
	static void SemaphoreInit(int32_t *semaphore, int32_t value);
	static void SignalSet(int32_t *semaphore);
	static void SignalUnset(int32_t *semaphore);
	static void SignalWait(int32_t *semaphore);
};

#endif
