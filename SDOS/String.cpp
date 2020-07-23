#include "String.hpp"
#include <stdio.h> 
#include <ctype.h>
	
String::String(const uint8_t* str)
{
	Clear();
	uint32_t len = CalculateLength((uint8_t*)str);
	for (uint32_t i = 0; i < len; i++)
		this->Buffer[i] = str[i];
	StrSize = len;
}

String::String(uint8_t* str)
{
	Clear();
	uint32_t len = CalculateLength((uint8_t*)str);
	for (uint32_t i = 0; i < len; i++)
		this->Buffer[i] = str[i];
	StrSize = len;
}

String::String(char* str)
{
	Clear();
	uint32_t len = CalculateLength((uint8_t*)str);
	for (uint32_t i = 0; i < len; i++)
		this->Buffer[i] = str[i];
	StrSize = len;
}

String::String(const char* str)
{
	Clear();
	uint32_t len = CalculateLength((uint8_t*)str);
	for (uint32_t i = 0; i < len; i++)
		this->Buffer[i] = str[i];
	StrSize = len;
}

String::String()
{
	Clear();
}

uint8_t* String::GetBuffer()
{
	return (uint8_t*)this->Buffer;
}

void String::Clear()
{
	for (uint32_t i = 0; i < BUFFER_SIZE; i++)
		Buffer[i] = 0;
	StrSize = 0;
}

void String::ToLower()
{
	for (uint32_t i = 0; i < StrSize; i++)
		Buffer[i] = tolower(Buffer[i]);
}

void String::ToUpper()
{
	for (uint32_t i = 0; i < StrSize; i++)
		Buffer[i] = toupper(Buffer[i]);
}

uint32_t String::GetLength()
{
	return StrSize;
}

uint32_t String::CalculateLength(uint8_t* str)
{
	uint32_t c = 0;
	while (str[c] != '\0')
	{
		c++;
		if (c >= BUFFER_SIZE)
		{
			str[BUFFER_SIZE - 1] = '\0';
			break;
		}
	}
	return c;
}

const uint8_t String::operator[](const uint32_t i) const 
{
	return Buffer[i];
}

uint8_t& String::operator[](const uint32_t i) 
{
	return Buffer[i]; 
}

String &String::operator=(const String &lr)
{
	Clear();
	for (uint32_t i = 0; i < lr.StrSize; i++)
		this->Buffer[i] = lr.Buffer[i];
	this->Buffer[lr.StrSize - 1] = '\0';
	this->StrSize = lr.StrSize;
	return *this;
}

String &String::operator=(const uint8_t* lr)
{
	Clear();
	uint32_t len = CalculateLength((uint8_t*)lr);
	for (uint32_t i = 0; i < len; i++)
		this->Buffer[i] = lr[i];
	this->Buffer[len - 1] = '\0';
	StrSize = len;
	return *this;
}

String &String::operator+=(const uint8_t &str)
{
	Buffer[StrSize] = str;
	StrSize++;
	if (StrSize >= BUFFER_SIZE)
	{
		StrSize = StrSize - 1;
		return *this;
	}
	return *this;
}

String &String::operator+=(const int &num)
{
	char buf[BUFFER_SIZE];
	snprintf(buf, BUFFER_SIZE, "%d", num);         //convert number to string
	uint32_t len = CalculateLength((uint8_t*)buf);
	for (uint32_t i = 0; i < len; i++)
		*this += buf[i];
	StrSize += len;
	return *this;
}

String &String::operator+=(const uint32_t &num)
{
	char buf[BUFFER_SIZE];
	snprintf(buf, BUFFER_SIZE, "%lu", num);
	uint32_t len = CalculateLength((uint8_t*)buf);
	for (uint32_t i = 0; i < len; i++)
		*this += buf[i];
	StrSize += len;
	return *this;
}

String &String::operator+=(const String &str)
{
	for (uint32_t i = 0; i < str.StrSize; i++)
		*this += str[i];
	StrSize += str.StrSize;
	return *this;
}

bool String::operator==(String &other) const
{
	if (this->StrSize != other.StrSize)
		return false;
	for (uint32_t i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other.Buffer[i])
			return false;
	return true;
}

bool String::operator==(const uint8_t* other) const
{
	if (this->StrSize != CalculateLength((uint8_t*)other))
		return false;
	for (uint32_t i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other[i])
			return false;
	return true;
}

bool String::operator==(const char* other) const
{
	if (this->StrSize != CalculateLength((uint8_t*)other))
		return false;
	for (uint32_t i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other[i])
			return false;
	return true;
}

void String::Update()
{
	this->StrSize = CalculateLength(this->Buffer);
}

bool String::operator==(uint8_t* other) const
{
	if (this->StrSize != CalculateLength(other))
		return false;
	for (uint32_t i = 0; i < this->StrSize; i++)
		if (this->Buffer[i] != other[i])
			return false;
	return true;
}
