//Still missing interrupt and EXTI handling

#ifndef _GPIO_H_
#define _GPIO_H_

#include "main.hpp"

enum GPIOMode
{
	INPUT,
	OUTPUT_PUSHPULL,
	OUTPUT_OPENDRAIN,
	ALTERNATE_PUSHPULL,
	ALTERNATE_OPENDRAIN,
	ANALOG, //kind of useful outside of ADC work, it completely opens up the pin
	INTERRUPT_RISING,
	INTERRUPT_FALLING,
	INTERRUPT_RISING_FALLING,
	EVENT_RISING,
	EVENT_FALLING,
	EVENT_RISING_FALLING
};
	
enum GPIOSpeed
{
	LOW,
	MEDIUM,
	HIGH,
	VERY_HIGH
};
	
enum GPIOPull
{
	NOPULL,
	PULLUP,
	PULLDOWN
};
	
enum GPIOAlternate
{
	NONE,
	AF0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15
};

class GPIOData;

class GPIOBase
{
public:
	static GPIOData* Get(uint32_t pin, char group);
	static GPIOData GroupA[16];
	static GPIOData GroupB[16];
	static GPIOData GroupC[16];
	static GPIOData GroupD[16];
	static GPIOData GroupE[16];
	static GPIOData GroupF[16];
};

class GPIOData
{
public:
	GPIOData* Mode(GPIOMode mode);
	GPIOData* Pull(GPIOPull pull);
	GPIOData* Speed(GPIOSpeed speed);
	GPIOData* Alternate(GPIOAlternate alternate);
	bool Read();
	GPIOData* SetLow();
	GPIOData* SetHigh();
	GPIOData* Toggle();
	GPIOData* Update();
	GPIOData* Lock();
	
	GPIOData(uint32_t pin, char group);
private:
	bool _init;
	char _group;
	uint32_t _pin;
	GPIOMode _mode;
	GPIOPull _pull;
	GPIOSpeed _speed;
	GPIOAlternate _alternate;
	GPIO_TypeDef* _gpio;
};

#endif
