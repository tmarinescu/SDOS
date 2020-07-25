#include "main.hpp"
#include "FPU.hpp"
#include "GPIO.hpp"
#include "Math.hpp"
#include "Float.hpp"

cvoid SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

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

	GPIOBase::Get(8, 'B')->Mode(GPIOMode::OUTPUT_PUSHPULL)->Speed(GPIOSpeed::HIGH)->Pull(GPIOPull::NOPULL)->Update()->Lock()->SetLow();
	
	__NOP();
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
