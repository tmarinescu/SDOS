#ifndef _CRITICAL_H_
#define _CRITICAL_H_

#include "main.hpp"

class Critical
{
public:
	static uint32_t DisableAllInterrupts();
	static void EnableAllInterrupts(uint32_t mem);
	static void EnableAllInterrupts();
	
	class Section
	{
	public:
		Section();
		~Section();
		void Enter();
		void Exit();
	
		bool Active;
		uint32_t Memory;
	};
};

#endif
