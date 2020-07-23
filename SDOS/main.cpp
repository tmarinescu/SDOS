#include "main.hpp"
#include "FPU.hpp"

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

	__GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_8;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	__NOP();
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_Delay(500);
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
