//Transient Obedience Based Y-Network System

#ifndef _TOBY_H_
#define _TOBY_H_

#include "main.hpp"

enum TOBYMsg
{
	InputJumpedUp,
	InputJumpedDown,
	InputOverHighLevel,
	InputUnderLowLevel,
	InputSensitive,
	WrongPrediction,
};

class TOBYFloat
{
public:
	bool ValidateInput(float input);
	void SetExpectedLevels(float lowLevel, float highLevel);
	void SetSensitivity(float sens);
	void SetJumpLimits(float down, float up);
	void BindTransientHandler(void(*func)(TOBYMsg, float, float));
private:
	float _buffer[256];
	uint32_t _bufferIndex;
	float _lastInput;
	float _futureInput;
	float _currentAverage;
	float _delta;
	uint32_t _size;
	void _shiftArray();
	void(*_suspectHandler)(TOBYMsg msg, float input, float expected);
};

class TOBYInt
{
public:
	bool ValidateInput(uint32_t input);
	void SetExpectedLevels(uint32_t lowLevel, uint32_t highLevel);
	void SetSensitivity(uint32_t sens);
	void SetJumpLimits(uint32_t down, uint32_t up);
	void BindTransientHandler(void(*func)(TOBYMsg, uint32_t, uint32_t));
	void SetTruncate();
	void SetRound();
private:
	bool _round;
	uint32_t _buffer[256];
	uint32_t _bufferIndex;
	uint32_t _lastInput;
	uint32_t _futureInput;
	uint32_t _currentAverage;
	uint32_t _delta;
	uint32_t _size;
	void _shiftArray();
	void(*_suspectHandler)(TOBYMsg msg, uint32_t input, uint32_t expected);
};

#endif

