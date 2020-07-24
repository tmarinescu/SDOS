#include "main.hpp"
#include "FPU.hpp"
#include "GPIO.hpp"

cvoid SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

extern "C"
{
	float x = 0.0f;
	float y = 4124.51f;
	float z = 0.0f;
	uint32_t f = 0;
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
		x = FPUnit::Sqrt(y);
		z = FPUnit::Mul(x, y);
		__NOP();
		f = FPUnit::Compare(0.126f, 0.127f);
		__NOP();
		x = FPUnit::ToFloat(5512);
		f = FPUnit::ToInt32(5222.51521f, true);
		__NOP();
	}
}
