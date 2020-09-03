#include "main.hpp"
#include "FPU.hpp"
#include "GPIO.hpp"
#include "Math.hpp"
#include "Float.hpp"
#include "Scheduler.hpp"
#include "System.hpp"

extern "C"
{
	Float x = 0.5f;
	Float y = 25.124f;
	float z = 16.16f;
}

int main(void)
{
	FPUnit::Enable();
	FPUnit::SelfTest();
	HAL_Init();
	__disable_irq();
	
	System::SetupSysTick();
	
	GPIOBase::Get(8, 'B')->Mode(GPIOMode::OUTPUT_PUSHPULL)->Speed(GPIOSpeed::HIGH)->Pull(GPIOPull::NOPULL)->Update()->Lock()->SetLow();

	for (;;)
	{
		GPIOBase::Get(8, 'B')->SetHigh();
		HAL_Delay(500);
		GPIOBase::Get(8, 'B')->SetLow();
		HAL_Delay(500);
		GPIOBase::Get(8, 'B')->Mode(GPIOMode::INPUT)->Speed(GPIOSpeed::LOW)->Pull(GPIOPull::PULLDOWN)->Update();
		
		x = z;
		x += z;
		y = x;
		y = Math::Sqrt(y);

		__NOP();
	}
}
