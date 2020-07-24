/*Floating-Point Unit System*/

#ifndef _FPU_H_
#define _FPU_H_

#include "main.hpp"

cfloat FPU_SQRT(float arg);
cvoid FPU_ENABLE(void);
cvoid FPU_DISABLE(void);
cfloat FPU_MUL(float arg1, float arg2);
cfloat FPU_DIV(float arg1, float arg2);
cfloat FPU_ADD(float arg1, float arg2);
cfloat FPU_SUB(float arg1, float arg2);
cfloat FPU_ABS(float arg1);
cuint32_t FPU_CMP(float arg1, float arg2);
cuint32_t FPU_CVT_TO_INT(float arg);
cuint32_t FPU_CVT_TO_INT_R(float arg);
cfloat FPU_CVT_TO_FLT(uint32_t arg);
cfloat FPU_NEG(float arg1);

class FPUnit
{
public:
	static bool SelfTest();
	static bool Enable();
	static bool Disable();
	static float Sqrt(float num);
	static float Mul(float num1, float num2);
	static float Div(float num1, float num2);
	static float Add(float num1, float num2);
	static float Sub(float num1, float num2);
	static float Abs(float num);
	static float ToFloat(uint32_t num);
	static uint32_t ToInt32(float num, bool round = true);
	static uint32_t Compare(float num1, float num2);  //0 = equal, 1 = num1 greater, 2 = num2 greater
	static float Neg(float num);
	static uint8_t GetError();
	
private:
	static bool _enabled;
	static uint8_t _lastError;
};


#endif
