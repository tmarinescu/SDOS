/*String Class*/

//Needs a review, this was written 2 years ago

#ifndef _STRING_H_
#define _STRING_H_

#include "main.hpp"

#define BUFFER_SIZE 64

class String
{
private:
	//Internals
	uint32_t StrSize = 0;
	uint8_t Buffer[BUFFER_SIZE];

public:
	//Constructors
	String();
	String(const uint8_t* str);
	String(uint8_t* str);
	String(char* str);
	String(const char* str);
	//Functionality
	void ToLower();
	void ToUpper();
	uint32_t GetLength();
	static uint32_t CalculateLength(uint8_t* str);
	void Clear();
	void Update();

	//Overload operators
	const uint8_t operator[](const uint32_t i) const;
	uint8_t& operator[](const uint32_t i);
	String &operator=(const String &lr);
	String &operator=(const uint8_t* lr);
	String &operator+=(const uint8_t &str);
	String &operator+=(const int &num);
	String &operator+=(const uint32_t &num);
	String &operator+=(const String &str);
	bool operator==(String &other) const;
	bool operator==(const uint8_t* other) const;
	bool operator==(const char* other) const;
	bool operator==(uint8_t* other) const;
	uint8_t* GetBuffer();
};

#endif