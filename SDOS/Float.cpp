#include "Float.hpp"
#include "FPU.hpp"

Float::Float(float f)
{
	_val = f;
}
	
Float Float::operator=(float const &lr)
{
	_val = lr;
	return *this;
}

Float Float::operator=(Float const &lr)
{
	_val = lr._val;
	return *this;
}

bool Float::operator>(float const &lr)
{
	return FPUnit::Compare(_val, lr) == 1;
}

bool Float::operator>(Float const &lr)
{
	return FPUnit::Compare(_val, lr._val) == 1;
}

bool Float::operator>=(float const &lr)
{
	return FPUnit::LSGreaterEqualRS(_val, lr);
}

bool Float::operator>=(Float const &lr)
{
	return FPUnit::LSGreaterEqualRS(_val, lr._val);
}
	
bool Float::operator<(float const &lr)
{
	return FPUnit::RSGreaterLS(_val, lr);
}

bool Float::operator<(Float const &lr)
{
	return FPUnit::RSGreaterLS(_val, lr._val);
}

bool Float::operator<=(float const &lr)
{
	return FPUnit::RSGreaterEqualLS(_val, lr);
}

bool Float::operator<=(Float const &lr)
{
	return FPUnit::RSGreaterEqualLS(_val, lr._val);
}
	
bool Float::operator==(float const &lr)
{
	return FPUnit::Compare(_val, lr) == 0;
}

bool Float::operator==(Float const &lr)
{
	return FPUnit::Compare(_val, lr._val) == 0;
}
	
Float Float::operator+(float const &lr)
{
	return FPUnit::Add(_val, lr);
}

Float Float::operator+(Float const &lr)
{
	return FPUnit::Add(_val, lr._val);
}
	
Float Float::operator+=(float const &lr)
{
	_val = FPUnit::Add(_val, lr);
	return *this;
}

Float Float::operator+=(Float const &lr)
{
	_val = FPUnit::Add(_val, lr._val);
	return *this;
}
	
Float Float::operator-(float const &lr)
{
	return FPUnit::Sub(_val, lr);
}

Float Float::operator-(Float const &lr)
{
	return FPUnit::Sub(_val, lr._val);
}
	
Float Float::operator-=(float const &lr)
{
	_val = FPUnit::Sub(_val, lr);
	return *this;
}

Float Float::operator-=(Float const &lr)
{
	_val = FPUnit::Sub(_val, lr._val);
	return *this;
}
	
Float Float::operator*(float const &lr)
{
	return FPUnit::Mul(_val, lr);
}

Float Float::operator*(Float const &lr)
{
	return FPUnit::Mul(_val, lr._val);
}
	
Float Float::operator*=(float const &lr)
{
	_val = FPUnit::Mul(_val, lr);
	return *this;
}

Float Float::operator*=(Float const &lr)
{
	_val = FPUnit::Mul(_val, lr._val);
	return *this;
}
	
Float Float::operator/(float const &lr)
{
	return FPUnit::Div(_val, lr);
}

Float Float::operator/(Float const &lr)
{
	return FPUnit::Div(_val, lr._val);
}
	
Float Float::operator/=(float const &lr)
{
	_val = FPUnit::Div(_val, lr);
	return *this;
}

Float Float::operator/=(Float const &lr)
{
	_val = FPUnit::Div(_val, lr._val);
	return *this;
}
	
Float::operator float()
{
	return _val;
}
