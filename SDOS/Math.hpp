#pragma once
#ifndef _MATH_H_
#define _MATH_H_

#include "main.hpp"

class Math
{
public:
	static float AddToAverage(float curAvg, float newVal, uint32_t curSize);
	static float SubFromAverage(float curAvg, float oldVal, uint32_t curSize);
	static float ReplaceInAverage(float curAvg, float oldVal, float newVal, uint32_t curSize);
	static float Sqrt(float num);
	static float Sqrt(uint32_t num);
};

#endif
