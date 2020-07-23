//Run-time Error Detective System

#ifndef _RED_H_
#define _RED_H_

#include "main.hpp"

class RED
{
public:
	static void DumpToLoRA(void);
	static void DumpToUART(void);
	static void DumpToCAN(void);
	static void DumpToI2C(void);
	static void DumpToMem(void); //External memory
	static void DumpToAll(void);
	static void AwaitForResponse(void);
	
private:
	/*Basic*/
	static bool		_hardwareError;
	static bool		_softwareError;
	static bool		_watchDogTrigger;
	static bool		 _bootLoop;
	static uint32_t _numBootFails;
	static uint32_t	_suspectedPeriph;
	
	/*OS Stack*/
	static uint32_t _stackSection;
	static uint32_t _stackLoc;
	static uint32_t _stackMax;
	static uint32_t _stackMin;
	static uint32_t _stackError;
	
	/*OS Basic*/
	static uint32_t _thread;
	static uint32_t _task;
	static uint32_t _iterations;
	
	/*CPU*/
	static uint32_t _frequency;
	static uint32_t _temp;
};

#endif
