#ifndef _FLOAT_H_
#define _FLOAT_H_

#include "main.hpp"

class Float
{
public:
	Float(float f);
	
	Float operator=(float const &lr);
	Float operator=(Float const &lr);
	
	bool operator>(float const &lr);
	bool operator>(Float const &lr);
	
	bool operator>=(float const &lr);
	bool operator>=(Float const &lr);
	
	bool operator<(float const &lr);
	bool operator<(Float const &lr);
	
	bool operator<=(float const &lr);
	bool operator<=(Float const &lr);
	
	bool operator==(float const &lr);
	bool operator==(Float const &lr);
	
	Float operator+(float const &lr);
	Float operator+(Float const &lr);
	
	Float operator+=(float const &lr);
	Float operator+=(Float const &lr);
	
	Float operator-(float const &lr);
	Float operator-(Float const &lr);
	
	Float operator-=(float const &lr);
	Float operator-=(Float const &lr);
	
	Float operator*(float const &lr);
	Float operator*(Float const &lr);
	
	Float operator*=(float const &lr);
	Float operator*=(Float const &lr);
	
	Float operator/(float const &lr);
	Float operator/(Float const &lr);
	
	Float operator/=(float const &lr);
	Float operator/=(Float const &lr);
	
	operator float();
	
private:
	float _val;
};

#endif