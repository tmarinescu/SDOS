#include "System.hpp"


void System::SetupSysTick(void)
{
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	//Check the programming manual, (SHPR3 PRI_15)
	SHPR3 = (SHPR3 & 0x00FFFFFF) | 0xE0000000;
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}