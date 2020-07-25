#include "Math.hpp"
#include "FPU.hpp"

float Math::AddToAverage(float curAvg, float newVal, uint32_t curSize)
{
	return FPUnit::Add(curAvg, FPUnit::Div(FPUnit::Sub(newVal, curAvg), FPUnit::ToFloat(curSize + 1)));
}

float Math::SubFromAverage(float curAvg, float oldVal, uint32_t curSize)
{
	return FPUnit::Add(curAvg, FPUnit::Div(FPUnit::Sub(curAvg, oldVal), FPUnit::ToFloat(curSize - 1)));
}

float Math::ReplaceInAverage(float curAvg, float oldVal, float newVal, uint32_t curSize)
{
	return FPUnit::Add(curAvg, FPUnit::Div(FPUnit::Add(curAvg, oldVal, FPUnit::Neg(newVal)), FPUnit::ToFloat(curSize - 1)));
}

float Math::Sqrt(float num)
{
	return FPUnit::Sqrt(num);
}

float Math::Sqrt(uint32_t num)
{
	return FPUnit::Sqrt(FPUnit::ToFloat(num));
}