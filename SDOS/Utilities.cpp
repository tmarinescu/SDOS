#include "Utilities.hpp"

void Utilities::SetRegisterQuad(volatile uint32_t* reg, uint32_t index, bool bit1, bool bit2, bool bit3, bool bit4)
{
	uint32_t tmp = (((bit1 << 3) | (bit2 << 2)) | (bit3 << 1)) | (bit4);
	tmp <<= index * 4U;
	*reg &= ~(15U << (index * 4U));
	*reg |= tmp;
}

void Utilities::SetRegisterDual(volatile uint32_t* reg, uint32_t index, bool bit1, bool bit2)
{
	uint32_t tmp = 2U * bit1 + bit2;
	tmp <<= index * 2U;
	*reg &= ~(3U << (index * 2U));
	*reg |= tmp;
}

void Utilities::SetRegisterSingle(volatile uint32_t* reg, uint32_t index, bool bit)
{
	if (bit)
		*reg |= 1U << index;
	else
		*reg &= ~(1U << index);
}

uint32_t Utilities::GetRegisterDual(volatile uint32_t* reg, uint32_t index)
{
	return 2U * (*reg & (index * 2U + 1)) + (*reg & (index * 2U));
}

uint32_t Utilities::GetRegisterSingle(volatile uint32_t* reg, uint32_t index)
{
	return *reg & index;
}