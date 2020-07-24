#include "GPIO.hpp"
#include "Utilities.hpp"

GPIOData* GPIOBase::Get(uint32_t pin, char group)
{
	if (group == 'A')
		return &GroupA[pin];
	else if (group == 'B')
		return &GroupB[pin];
	else if (group == 'C')
		return &GroupC[pin];
	else if (group == 'D')
		return &GroupD[pin];
	else if (group == 'E')
		return &GroupE[pin];
	else if (group == 'F')
		return &GroupF[pin];
	else
		return &GroupA[pin];
}

GPIOData GPIOBase::GroupA[16] = 
{
	GPIOData(0, 'A'),
	GPIOData(1, 'A'),
	GPIOData(2, 'A'),
	GPIOData(3, 'A'),
	GPIOData(4, 'A'),
	GPIOData(5, 'A'),
	GPIOData(6, 'A'),
	GPIOData(7, 'A'),
	GPIOData(8, 'A'),
	GPIOData(9, 'A'),
	GPIOData(10, 'A'),
	GPIOData(11, 'A'),
	GPIOData(12, 'A'),
	GPIOData(13, 'A'),
	GPIOData(14, 'A'),
	GPIOData(15, 'A') 
};

GPIOData GPIOBase::GroupB[16] = 
{
	GPIOData(0, 'B'),
	GPIOData(1, 'B'),
	GPIOData(2, 'B'),
	GPIOData(3, 'B'),
	GPIOData(4, 'B'),
	GPIOData(5, 'B'),
	GPIOData(6, 'B'),
	GPIOData(7, 'B'),
	GPIOData(8, 'B'),
	GPIOData(9, 'B'),
	GPIOData(10, 'B'),
	GPIOData(11, 'B'),
	GPIOData(12, 'B'),
	GPIOData(13, 'B'),
	GPIOData(14, 'B'),
	GPIOData(15, 'B') 
};
GPIOData GPIOBase::GroupC[16] = 
{
	GPIOData(0, 'C'),
	GPIOData(1, 'C'),
	GPIOData(2, 'C'),
	GPIOData(3, 'C'),
	GPIOData(4, 'C'),
	GPIOData(5, 'C'),
	GPIOData(6, 'C'),
	GPIOData(7, 'C'),
	GPIOData(8, 'C'),
	GPIOData(9, 'C'),
	GPIOData(10, 'C'),
	GPIOData(11, 'C'),
	GPIOData(12, 'C'),
	GPIOData(13, 'C'),
	GPIOData(14, 'C'),
	GPIOData(15, 'C') 
};

GPIOData GPIOBase::GroupD[16] = 
{
	GPIOData(0, 'D'),
	GPIOData(1, 'D'),
	GPIOData(2, 'D'),
	GPIOData(3, 'D'),
	GPIOData(4, 'D'),
	GPIOData(5, 'D'),
	GPIOData(6, 'D'),
	GPIOData(7, 'D'),
	GPIOData(8, 'D'),
	GPIOData(9, 'D'),
	GPIOData(10, 'D'),
	GPIOData(11, 'D'),
	GPIOData(12, 'D'),
	GPIOData(13, 'D'),
	GPIOData(14, 'D'),
	GPIOData(15, 'D') 
};

GPIOData GPIOBase::GroupE[16] = 
{
	GPIOData(0, 'E'),
	GPIOData(1, 'E'),
	GPIOData(2, 'E'),
	GPIOData(3, 'E'),
	GPIOData(4, 'E'),
	GPIOData(5, 'E'),
	GPIOData(6, 'E'),
	GPIOData(7, 'E'),
	GPIOData(8, 'E'),
	GPIOData(9, 'E'),
	GPIOData(10, 'E'),
	GPIOData(11, 'E'),
	GPIOData(12, 'E'),
	GPIOData(13, 'E'),
	GPIOData(14, 'E'),
	GPIOData(15, 'E') 
};

GPIOData GPIOBase::GroupF[16] = 
{
	GPIOData(0, 'F'),
	GPIOData(1, 'F'),
	GPIOData(2, 'F'),
	GPIOData(3, 'F'),
	GPIOData(4, 'F'),
	GPIOData(5, 'F'),
	GPIOData(6, 'F'),
	GPIOData(7, 'F'),
	GPIOData(8, 'F'),
	GPIOData(9, 'F'),
	GPIOData(10, 'F'),
	GPIOData(11, 'F'),
	GPIOData(12, 'F'),
	GPIOData(13, 'F'),
	GPIOData(14, 'F'),
	GPIOData(15, 'F') 
};

GPIOData* GPIOData::Mode(GPIOMode mode)
{
	_mode = mode;
	return this;
}

GPIOData* GPIOData::Pull(GPIOPull pull)
{
	_pull = pull;
	return this;
}

GPIOData* GPIOData::Speed(GPIOSpeed speed)
{
	_speed = speed;
	return this;
}

GPIOData* GPIOData::Alternate(GPIOAlternate alternate)
{
	_alternate = alternate;
	return this;
}

bool GPIOData::Read()
{
	if (_init)
		return Utilities::GetRegisterSingle(&_gpio->IDR, _pin);
	return false;
}

GPIOData* GPIOData::SetLow()
{
	if (_init)
		Utilities::SetRegisterSingle(&_gpio->BRR, _pin, 1);
	return this;
}

GPIOData* GPIOData::SetHigh()
{
	if(_init)
		Utilities::SetRegisterSingle(&_gpio->BSRR, _pin, 1);
	return this;
}

GPIOData* GPIOData::Toggle()
{
	if (_init)
	{
		if (Utilities::GetRegisterSingle(&_gpio->ODR, _pin))
		{
			Utilities::SetRegisterSingle(&_gpio->BRR, _pin, 1);
		}
		else
		{
			Utilities::SetRegisterSingle(&_gpio->BSRR, _pin, 1);
		}
	}
	return this;
}

GPIOData* GPIOData::Update()
{
	if (_gpio == GPIOA)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	else if (_gpio == GPIOB)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	else if (_gpio == GPIOC)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	else if (_gpio == GPIOD)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	else if (_gpio == GPIOC)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	else if (_gpio == GPIOE)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	else if (_gpio == GPIOF)
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
	
	if (_mode == GPIOMode::OUTPUT_OPENDRAIN || _mode == GPIOMode::ALTERNATE_OPENDRAIN)
	{
		if (_speed == GPIOSpeed::LOW)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 0, 0);
		else if (_speed == GPIOSpeed::MEDIUM)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 0, 1);
		else if (_speed == GPIOSpeed::HIGH)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 1, 0);
		else if (_speed == GPIOSpeed::VERY_HIGH)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 1, 1);
		
		Utilities::SetRegisterSingle(&_gpio->OTYPER, _pin, 1);
	}
	else if (_mode == GPIOMode::OUTPUT_PUSHPULL || _mode == GPIOMode::ALTERNATE_PUSHPULL)
	{
		if (_speed == GPIOSpeed::LOW)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 0, 0);
		else if (_speed == GPIOSpeed::MEDIUM)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 0, 1);
		else if (_speed == GPIOSpeed::HIGH)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 1, 0);
		else if (_speed == GPIOSpeed::VERY_HIGH)
			Utilities::SetRegisterDual(&_gpio->OSPEEDR, _pin, 1, 1);
		
		Utilities::SetRegisterSingle(&_gpio->OTYPER, _pin, 0);
	}
	
	if (_pull == GPIOPull::NOPULL)
		Utilities::SetRegisterDual(&_gpio->PUPDR, _pin, 0, 0);
	else if (_pull == GPIOPull::PULLUP)
		Utilities::SetRegisterDual(&_gpio->PUPDR, _pin, 0, 1);
	else
		Utilities::SetRegisterDual(&_gpio->PUPDR, _pin, 1, 0);
	
	if (_alternate != GPIOAlternate::NONE)
	{
		if (_pin <= 7)
		{
			if (_alternate == AF0)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 0, 0, 0);
			else if (_alternate == AF1)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 0, 0, 1);
			else if (_alternate == AF2)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 0, 1, 0);
			else if (_alternate == AF3)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 0, 1, 1);
			else if (_alternate == AF4)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 1, 0, 0);
			else if (_alternate == AF5)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 1, 0, 1);
			else if (_alternate == AF6)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 1, 1, 0);
			else if (_alternate == AF7)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 0, 1, 1, 1);
			else if (_alternate == AF8)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 0, 0, 0);
			else if (_alternate == AF9)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 0, 0, 1);
			else if (_alternate == AF10)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 0, 1, 0);
			else if (_alternate == AF11)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 0, 1, 1);
			else if (_alternate == AF12)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 1, 0, 0);
			else if (_alternate == AF13)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 1, 0, 1);
			else if (_alternate == AF14)
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 1, 1, 0);
			else
				Utilities::SetRegisterQuad(&_gpio->AFR[0], _pin, 1, 1, 1, 1);
		}
		else
		{
			if (_alternate == AF0)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 0, 0, 0);
			else if (_alternate == AF1)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 0, 0, 1);
			else if (_alternate == AF2)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 0, 1, 0);
			else if (_alternate == AF3)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 0, 1, 1);
			else if (_alternate == AF4)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 1, 0, 0);
			else if (_alternate == AF5)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 1, 0, 1);
			else if (_alternate == AF6)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 1, 1, 0);
			else if (_alternate == AF7)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 0, 1, 1, 1);
			else if (_alternate == AF8)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 0, 0, 0);
			else if (_alternate == AF9)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 0, 0, 1);
			else if (_alternate == AF10)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 0, 1, 0);
			else if (_alternate == AF11)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 0, 1, 1);
			else if (_alternate == AF12)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 1, 0, 0);
			else if (_alternate == AF13)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 1, 0, 1);
			else if (_alternate == AF14)
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 1, 1, 0);
			else
				Utilities::SetRegisterQuad(&_gpio->AFR[1], _pin, 1, 1, 1, 1);
		}
	}
	
	if (_mode == GPIOMode::INPUT)
	{
		Utilities::SetRegisterDual(&_gpio->MODER, _pin, 0, 0);
	}
	else if (_mode == GPIOMode::OUTPUT_OPENDRAIN || _mode == GPIOMode::OUTPUT_PUSHPULL)
	{
		Utilities::SetRegisterDual(&_gpio->MODER, _pin, 0, 1);
	}
	else if (_mode == GPIOMode::ALTERNATE_OPENDRAIN || _mode == GPIOMode::ALTERNATE_PUSHPULL)
	{
		Utilities::SetRegisterDual(&_gpio->MODER, _pin, 1, 0);
	}
	else if (_mode == GPIOMode::ANALOG)
	{
		Utilities::SetRegisterDual(&_gpio->MODER, _pin, 1, 1);
	}
	
	_init = true;
	return this;
}

GPIOData::GPIOData(uint32_t pin, char group)
{
	_pin = pin;
	_group = group;
	if (group == 'A')
		_gpio = GPIOA;
	else if (group == 'B')
		_gpio = GPIOB;
	else if (group == 'C')
		_gpio = GPIOC;
	else if (group == 'D')
		_gpio = GPIOD;
	else if (group == 'E')
		_gpio = GPIOE;
	else if (group == 'F')
		_gpio = GPIOF;
	else
		_gpio = GPIOA;
	
	_init = false;
	_mode = GPIOMode::INPUT;
	_pull = GPIOPull::PULLDOWN;
	_speed = GPIOSpeed::LOW;
	_alternate = GPIOAlternate::NONE;
}