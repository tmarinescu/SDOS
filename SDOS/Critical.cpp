#include "Critical.hpp"

uint32_t Critical::DisableAllInterrupts()
{
	//Existing interrupt mask
	uint32_t mask = __get_PRIMASK();
	__disable_irq();
	return mask;
}

void Critical::EnableAllInterrupts(uint32_t mem)
{
	if (!mem)
		__enable_irq();
}

void Critical::EnableAllInterrupts()
{
	__enable_irq();
}

Critical::Section::Section()
{
	Active = false;
	Memory = 0;
}

Critical::Section::~Section()
{
	if (Active)
	{
		EnableAllInterrupts(Memory);
		//StartSwitching();
	}
}

void Critical::Section::Enter()
{
	if (!Active)
	{
		//StopSwitching();
		Active = true;
		Memory = DisableAllInterrupts();
	}
}

void Critical::Section::Exit()
{
	if (Active)
	{
		EnableAllInterrupts(Memory);
		Active = false;
		//StartSwitching();
	}
}