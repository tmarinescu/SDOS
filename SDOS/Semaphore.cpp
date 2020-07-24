#include "Semaphore.hpp"

void Semaphore::SemaphoreInit(int32_t *semaphore, int32_t value) //1 = unlocked, 0 = locked
{
	uint32_t prim = Critical::DisableAllInterrupts();
	*semaphore = value;
	Critical::EnableAllInterrupts(prim);
}

void Semaphore::SignalSet(int32_t *semaphore) //Signals for it to go through
{
	uint32_t prim = Critical::DisableAllInterrupts();
	*semaphore += 1;
	Critical::EnableAllInterrupts(prim);
}
	
void Semaphore::SignalUnset(int32_t *semaphore) //Sets the semaphore to blocking mode
{
	uint32_t prim = Critical::DisableAllInterrupts();
	*semaphore = 0;
	Critical::EnableAllInterrupts(prim);
}

void Semaphore::SignalWait(int32_t *semaphore) //Waits for semaphore to be set
{
	uint32_t prim = Critical::DisableAllInterrupts();
	while (*semaphore <= 0)
	{
		//Yield();
	}
		
	*semaphore -= 1;
	Critical::EnableAllInterrupts(prim);
}
