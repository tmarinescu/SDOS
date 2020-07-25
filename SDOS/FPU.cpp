#include "FPU.hpp"

bool FPUnit::SelfTest()
{
	__disable_irq();
	if (_enabled)
	{
		float set1 = 1234.5678f; //1234.56775
		float set2 = 5555.555; //5555.55518
		float set3 = Add(set1, set2);
		uint32_t cmp = Compare(set3, 6790.12305f);
		if (cmp == 0)
		{
			__NOP();
			float set4 = Mul(set1, set2);
			float set5 = Neg(set4);
			uint32_t cmp2 = Compare(set4, Abs(set5));
			if (cmp2 == 0)
			{
				__NOP();
			}
			else
			{
				__NOP();
			}
		}
		else
		{
			__NOP();
		}
		
		_lastError = 0;
		__enable_irq();
		return true;
	}
	_lastError = 124;
	__enable_irq();
	return false;
}

bool FPUnit::Disable()
{
	if (_enabled)
	{
		FPU_DISABLE();
		_enabled = false;
		_lastError = 0;
		return true;
	}
	else
	{
		_lastError = 1;
		return false;
	}
}

bool FPUnit::Enable()
{
	if (!_enabled)
	{
		FPU_ENABLE();
		_enabled = true;
		_lastError = 0;
		return true;
	}
	else
	{
		_lastError = 2;
		return false;
	}
}

float FPUnit::Sqrt(float num)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_SQRT(num);
	}
	else
	{
		_lastError = 3;
		return 0.0f;
	}
}

float FPUnit::Mul(float num1, float num2)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_MUL(num1, num2);
	}
	else
	{
		_lastError = 4;
		return 0.0f;
	}
}

float FPUnit::Div(float num1, float num2)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_DIV(num1, num2);
	}
	else
	{
		_lastError = 5;
		return 0.0f;
	}
}

float FPUnit::Add(float num1, float num2)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_ADD(num1, num2);
	}
	else
	{
		_lastError = 6;
		return 0.0f;
	}
}

float FPUnit::Add(float num1, float num2, float num3)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_ADD3(num1, num2, num3);
	}
	else
	{
		_lastError = 7;
		return 0.0f;
	}
}

float FPUnit::Add(float num1, float num2, float num3, float num4)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_ADD4(num1, num2, num3, num4);
	}
	else
	{
		_lastError = 8;
		return 0.0f;
	}
}

float FPUnit::Sub(float num1, float num2)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_SUB(num1, num2);
	}
	else
	{
		_lastError = 9;
		return 0.0f;
	}
}

float FPUnit::Sub(float num1, float num2, float num3)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_SUB3(num1, num2, num3);
	}
	else
	{
		_lastError = 10;
		return 0.0f;
	}
}

float FPUnit::Sub(float num1, float num2, float num3, float num4)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_SUB4(num1, num2, num3, num4);
	}
	else
	{
		_lastError = 11;
		return 0.0f;
	}
}

float FPUnit::Abs(float num)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_ABS(num);
	}
	else
	{
		_lastError = 12;
		return 0.0f;
	}
}

uint32_t FPUnit::Compare(float num1, float num2)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_CMP(num1, num2);
	}
	else
	{
		_lastError = 13;
		return 0;
	}
}

bool FPUnit::LSGreaterRS(float num1, float num2)
{
	return Compare(num1, num2) == 1;
}

bool FPUnit::RSGreaterLS(float num1, float num2)
{
	return Compare(num1, num2) == 2;
}

bool FPUnit::Equal(float num1, float num2)
{
	return Compare(num1, num2) == 0;
}

bool FPUnit::LSGreaterEqualRS(float num1, float num2)
{
	return FPU_CMP_GE(num1, num2) == 0;
}

bool FPUnit::RSGreaterEqualLS(float num1, float num2)
{
	return FPU_CMP_LE(num1, num2) == 0;
}

float FPUnit::ToFloat(uint32_t num)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_CVT_TO_FLT(num);
	}
	else
	{
		_lastError = 14;
		return 0.0f;
	}
}

uint32_t FPUnit::ToInt32(float num, bool round)
{
	if (_enabled)
	{
		_lastError = 0;
		if (round)
			return FPU_CVT_TO_INT_R(num);
		else
			return FPU_CVT_TO_INT(num);
	}
	else
	{
		_lastError = 15;
		return 0.0f;
	}
}

float FPUnit::Neg(float num)
{
	if (_enabled)
	{
		_lastError = 0;
		return FPU_NEG(num);
	}
	else
	{
		_lastError = 16;
		return 0.0f;
	}
}

uint8_t FPUnit::GetError()
{
	return _lastError;
}

bool FPUnit::_enabled = false;
uint8_t FPUnit::_lastError = 0;