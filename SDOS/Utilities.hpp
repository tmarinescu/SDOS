#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "main.hpp"

class Utilities
{
public:
	static void SetRegisterQuad(volatile uint32_t* reg, uint32_t index, bool bit1, bool bit2, bool bit3, bool bit4);
	static void SetRegisterDual(volatile uint32_t* reg, uint32_t index, bool bit1, bool bit2);
	static void SetRegisterMultiple(volatile uint32_t* reg, uint32_t index1, bool bit1, uint32_t index2, bool bit2);
	static void SetRegisterMultipleMono(volatile uint32_t* reg, uint32_t index1, bool bit1, uint32_t index2, bool bit2);
	static void SetRegisterSingle(volatile uint32_t* reg, uint32_t index, bool bit);

	static uint32_t GetRegisterSingle(volatile uint32_t* reg, uint32_t index);
	static uint32_t GetRegisterDual(volatile uint32_t* reg, uint32_t index);
};

#endif