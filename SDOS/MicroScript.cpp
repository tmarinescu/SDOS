/*TM*/
/*MicroScript V0.1*/

/* DATA LOG:
 * NOT FINISHED!!! (Missing a ton of functionality and features, error catching isn't complete)
 * NEEDS TO BE OPTIMIZED BETTER! (Some of these functions will wreck the stack due to a large influx of local variables, some logic and loops can also be combined/fixed)
 * 
 * A decision needs to be made about the architecture, as of right now this does NOT support multi-thread.
 * My current thinking is that this will only ever execute on one thread so multi-thread support isn't required.
 * (Before I forget ^ split the class into two, base and functions, pass base data to functions whenever that way you can create as many instances of the base and keep code/ram size to a minimum.)
 */

/* SCRIPT DESIGN */
/*
(all variables are global)
(@ denotes end of script)
(| denotes line break, script works by lines)

x0 => global variable x set to 0
y555 => global variable y set to 555
x+=2 => add 2 to x
y+=x => add x to y
x-=y => sub y from x
x|=y => bit OR x with y
x>=4 => bitwise shift x right by 4
x==y => set x to y
x==5 => set x to 5
x!!y => swap x and y

g => gpio
	=>g:a:12:e:4 (gpioa, pin 12, enabled, config 4)
	=>g:a:12:t (toggle gpioa pin 12)
	=>g:a:12:d (disable gpioa pin 12)
	=>g:a:y:x (sets gpioa pin y to state of x)
	=>g:a:y:x:z (sets gpioa pin y to sate of x with config of z)

d => debug
	=>d:x (debug x variable)

r => return
	=>r:x (return x variable)
	=>r:x:y:z (returns x, y, and z)

p => process
	=>p:1 (executes line 1 but goes back to where it was)

s => skip
	=>s:1 (skips to line 1)

i => if statement
	=>i:x>2 (if x greater than 2)

c => content block
	=>c:x+=2 (done after if statements)
	=>c:i:x>4 (nested if statement)
	=>c:c:x+=4 (nested context block)

e => else if statement
	=>e:x<2 (else x less than 2)
	=>e:# (else default)

e(single) => end
	=>e (done at the end of if statements)

a => assign variable
	=>a:x:g:a:12 (set x to gpioa pin 12)
	=>a:x:1:0 (set x to can bus message index 0)
	=>a:x:2:0 (set x to i2c message index 0)

b => backup variable
	=>b:x (backs up x, only one backup allowed per variable, x remains the same)

l => load backup into variable again
	=>l:x (non existent backups will be given a value of 0)
	=>l:x:y (load backup of y into x)

1 => can bus control
	1:45:45:0:1233:41 (send 45,45,0,1233,41 as a can packet)
	1:r (receive next can packet, dumps existing one)

2 => i2c control
	2:a:b:c:d (send abcd as an i2c packet) (letters converted to numbers)
	2:123:a:555
	2:r (receive next i2c packet)

3 => uart control
	3:a:b:c:d (same as i2c)

w => wait
	w:1000 (script pauses for 1000 milliseconds)
*/

#include "MicroScript.hpp"
#include <ctype.h>

int8_t MSEngine::_data[SCRIPT_MAX_SIZE];
//int8_t MSEngine::_dataBegins[SCRIPT_VAR_SIZE];
//int8_t MSEngine::_dataEnds[SCRIPT_VAR_SIZE];
int8_t MSEngine::_gVars[SCRIPT_VAR_SIZE];
int32_t MSEngine::_gVals[SCRIPT_VAR_SIZE];
int32_t MSEngine::_gValsBack[SCRIPT_VAR_SIZE];
int8_t MSEngine::_gLine = -1;
int32_t MSEngine::_dataLen = 0;
int32_t MSEngine::_numSkips = 0;

void MSEngine::Initialize()
{
	for (int32_t i = 0; i < SCRIPT_MAX_SIZE; i++)
	{
		_data[i] = -1;
	}
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		//_dataBegins[i] = -1;
		//_dataEnds[i] = -1;
		_gVars[i] = -1;
		_gVals[i] = 0;
	}
}

bool MSEngine::AnalyzeScript(const uint8_t* data)
{
	int32_t length = 0;
	//printf("Original Data: ");
	for(int32_t i = 0 ;  ; i++)
	{
		if (i >= SCRIPT_MAX_SIZE)
		{
			length = -1;
			break;
		}

		if (data[i] == '@')
		{
			length = i;
			break;
		}
		//printf("%c", data[i]);
	}
	//printf("\n\n");
	if(length == -1)
	{
		//printf("Error: Script is too large.\n");
	}
	else if(length == 0)
	{
		//printf("Error: End of script not found. (Missing @)\n");
	}
	else
	{
		_dataLen = length;
		//printf("Script Size: %d\n", length);
	}

	//printf("\nScript Data: ");
	for(int32_t i = 0 ; i <= length ; i++)
	{
		_data[i] = data[i];
		if (_data[i] == '|')
		{
			//printf(" ");
		}
		//printf("%c", _data[i]);
		if(_data[i] == '|')
		{
			//printf(" ");
		}
	}
	//printf("\n\n");

	int32_t start = 0;
	int32_t end = 0;
	int32_t line = 1;
	bool failed = false;
	_gLine = 1;
	for (int32_t i = 0; i <= length; i++)
	{
		if (_data[i] == '|' || _data[i] == '@')
		{
			if (_numSkips == 0)
			{
				end = i;
				if (ExecuteLine(start, end))
				{
					start = i + 1;
				}
				else
				{
					//printf("Error on line %d\n", line);
					failed = true;
					break;
				}
			}
			else
			{
				start = i + 1;
				_numSkips--;
				//printf("[Line %d][DEBUG] Skipped line\n", line);
			}
			line++;
			_gLine = line;
		}
	}
	return true;
}

bool MSEngine::AnalyzeScript(String* data)
{
	int32_t length = 0;
	//printf("Original Data: ");
	for(uint32_t i = 0 ;  ; i++)
	{
		if (i >= SCRIPT_MAX_SIZE)
		{
			length = -1;
			break;
		}

		if ((*data)[i] == '@') //uh sure
			{
				length = i;
				break;
			}
		//printf("%c", data[i]);
	}
	//printf("\n\n");
	if(length == -1)
	{
		//printf("Error: Script is too large.\n");
	}
	else if(length == 0)
	{
		//printf("Error: End of script not found. (Missing @)\n");
	}
	else
	{
		_dataLen = length;
		//printf("Script Size: %d\n", length);
	}

	//printf("\nScript Data: ");
	for(int32_t i = 0 ; i <= length ; i++)
	{
		_data[i] = (*data)[i];
		if (_data[i] == '|')
		{
			//printf(" ");
		}
		//printf("%c", _data[i]);
		if(_data[i] == '|')
		{
			//printf(" ");
		}
	}
	//printf("\n\n");

	int32_t start = 0;
	int32_t end = 0;
	int32_t line = 1;
	bool failed = false;
	_gLine = 1;
	for (int32_t i = 0; i <= length; i++)
	{
		if (_data[i] == '|' || _data[i] == '@')
		{
			if (_numSkips == 0)
			{
				end = i;
				if (ExecuteLine(start, end))
				{
					start = i + 1;
				}
				else
				{
					//printf("Error on line %d\n", line);
					failed = true;
					break;
				}
			}
			else
			{
				start = i + 1;
				_numSkips--;
				//printf("[Line %d][DEBUG] Skipped line\n", line);
			}
			line++;
			_gLine = line;
		}
	}
	return true;
}

bool MSEngine::ExecuteLine(int32_t start, int32_t end)
{
	if ((end - start) == 1) //single letter command
		{
			if (_data[start] == 'e')
			{
				//printf("[Line %d][DEBUG] If statement closing\n", _gLine);
			}
		}
	else if ((end - start) == 3) //simple command
		{
			if (_data[start] == 'w')
				WaitCommand(start, end);
			else if (_data[start] == 'd')
				DebugCommand(start, end);
			else if (_data[start] == 'p')
				ProcessCommand(start, end);
			else if (_data[start] == 'l')
				LoadCommand(start, end);
			else if (_data[start] == 'b')
				BackupCommand(start, end);
			else if (_data[start] == 's')
				SkipCommand(start, end);
			//else
				//printf("[Line %d][DEBUG] Unknown command %c\n", _gLine, _data[start]);

		}
	else
	{
		if (_data[start + 1] != ':') //variable assignment
			{
				if (isdigit(_data[start + 1]) || _data[start + 1] == '-')
				{
					if ((end - start) > 3)
					{
						if (_data[start + 2] == '=')
						{
							SubCommand(start, end);
						}
						else
						{
							CreateVariableCommand(start, end);
						}
					}
					else
					{
						CreateVariableCommand(start, end);
					}
				}
				else
				{
					if (_data[start + 1] == '+')
					{
						AddCommand(start, end);
					}
					else if (_data[start + 1] == '-')
					{
						SubCommand(start, end);
					}
					else if (_data[start + 1] == '=')
					{
						EquCommand(start, end);
					}
					else if (_data[start + 1] == '*')
					{
						MulCommand(start, end);
					}
					else if (_data[start + 1] == '/')
					{
						DivCommand(start, end);
					}
				}
			}
		else //heavy command
			{
				if (_data[start] == 'g')
				{
					GPIOCommand(start, end);
				}
				else if (_data[start] == 'i')
				{
					if (IfCommand(start, end))
					{

					}
					else
					{
						int32_t skips = 0;
						int32_t enclosed = 0;
						for (int32_t i = end; i <= _dataLen; i++)
						{
							if ((_data[i] == '|' || _data[i] == '@') && (i <= _dataLen - 2))
							{
								if (_data[i + 1] == 'i')
								{
									enclosed++;
									skips++;
								}
								else if (_data[i + 1] == 'e')
								{
									if (_data[i + 2] == '|' || _data[i + 2] == '@')
									{
										if (enclosed == 0)
										{
											skips++;
											//printf("	=>[DEBUG] End detected on line [%d]\n", _gLine + skips);
											break;
										}
										else
										{
											enclosed--;
										}
									}
								}
								else
								{
									skips++;
								}
							}
						}
						_numSkips = skips;
					}
				}
			}
	}
	return true;
}

int32_t MSEngine::GetNumber(int32_t start, int32_t end, uint8_t* data = 0)
{
	int32_t number = 0;
	if (data == 0)
	{
		int32_t mult = 1;
		int32_t n = (end - start);
		n = n < 0 ? -n : n; /* quick absolute value check  */

		/* for each uint8_tacter in array */
		while (n--)
		{
			/* if not digit or '-', check if number > 0, break or continue */
			if ((_data[n + start] < '0' || _data[n + start] > '9') && _data[n + start] != '-') {
				if (number)
					break;
				else
					continue;
			}

			if (_data[n + start] == '-') {
				/* if '-' if number, negate, break */
				if (number) {
					number = -number;
					break;
				}
			}
			else {
				/* convert digit to numeric value   */
				number += (_data[n + start] - '0') * mult;
				mult *= 10;
			}
		}
	}
	else
	{
		int32_t mult = 1;
		int32_t n = end;
		n = n < 0 ? -n : n; /* quick absolute value check  */

		/* for each uint8_tacter in array */
		while (n--)
		{
			/* if not digit or '-', check if number > 0, break or continue */
			if ((data[n] < '0' || data[n] > '9') && data[n] != '-') {
				if (number)
					break;
				else
					continue;
			}

			if (data[n] == '-') {
				/* if '-' if number, negate, break */
				if (number) {
					number = -number;
					break;
				}
			}
			else {
				/* convert digit to numeric value   */
				number += (data[n] - '0') * mult;
				mult *= 10;
			}
		}
	}

	return number;
}

int32_t MSEngine::GetStartIndexByLine(int32_t line)
{
	int32_t curLine = 1;
	for (int32_t i = 0; i < _dataLen; i++)
	{
		if (curLine == line)
		{
			return i;
		}
		if (_data[i] == '|' || _data[i] == '@')
		{
			curLine++;
		}
	}
	return -1;
}

int32_t MSEngine::GetEndIndexByStartIndex(int32_t index)
{
	for (int32_t i = index; i < _dataLen; i++)
	{
		if (_data[i] == '|' || _data[i] == '@')
		{
			return i;
		}
	}
	return -1;
}

void MSEngine::WaitCommand(int32_t start, int32_t end)
{
	int32_t num = -1;
	for (int32_t i = 0; i < end; i++)
	{
		if (_data[i] == ':')
		{
			num = GetNumber(i + 1, end);
			break;
		}
	}

	if (num != -1)
	{
		//Execute wait
	}
}

void MSEngine::DebugCommand(int32_t start, int32_t end)
{
	uint8_t var = _data[start + 2];
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == var)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		//printf("Error on line %d: Variable %c is unknown.\n", _gLine, var);
	}
	else
	{
		//printf("[Line %d][DEBUG] Variable[%c] <=> Value[%d]\n", _gLine, _gVars[index], _gVals[index]);
	}
}

void MSEngine::SkipCommand(int32_t startIndex, int32_t endIndex)
{
	for (int32_t i = startIndex; i < endIndex; i++)
	{
		if (_data[startIndex] == ':')
		{
			_gLine = GetNumber(i + 1, endIndex);
		}
	}
}

void MSEngine::ProcessCommand(int32_t startIndex, int32_t endIndex)
{
	for (int32_t i = startIndex; i < endIndex; i++)
	{
		if (_data[startIndex] == ':')
		{
			_gLine = GetNumber(i + 1, endIndex);
			int32_t index = GetStartIndexByLine(_gLine);
			int32_t end = GetEndIndexByStartIndex(index);
			ExecuteLine(index, end);
		}
	}
}

void MSEngine::ReturnOneCommand(int32_t startIndex, int32_t endIndex)
{

}

void MSEngine::ReturnAllCommand(int32_t startIndex, int32_t endIndex)
{

}

void MSEngine::BackupCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex + 2];
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			_gValsBack[i] = _gVals[i];
			return;
		}
	}

	//printf("Error on line %d: Variable %c does not exist.\n", _gLine, val);
}

void MSEngine::LoadCommand(int32_t startIndex, int32_t endIndex)
{
	if ((endIndex - startIndex) == 3)
	{
		uint8_t val = _data[startIndex + 2];
		for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
		{
			if (_gVars[i] == val)
			{
				_gVals[i] = _gValsBack[i];
				return;
			}
		}

		//printf("Error on line %d: Variable %c does not exist.\n", _gLine, val);
	}
	else
	{
		uint8_t val = _data[startIndex + 2];
		int32_t index = -1;
		for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
		{
			if (_gVars[i] == val)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			//printf("Error on line %d: Variable %c does not exist.\n", _gLine, val);
		}
		else
		{
			uint8_t valNew = _data[startIndex + 4];
			for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
			{
				if (_gVars[i] == valNew)
				{
					_gVals[i] = _gValsBack[index];
					return;
				}
			}

			//printf("Error on line %d: Variable %c does not exist.\n", _gLine, val);
		}
	}
}

void MSEngine::CreateVariableCommand(int32_t startIndex, int32_t endIndex)
{
	int32_t num = GetNumber(startIndex + 1, endIndex);
	uint8_t val = _data[startIndex];
	bool exists = false;
	int32_t indexFound = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			exists = true;
			break;
		}
		if (indexFound == -1)
		{
			if (_gVars[i] == -1)
			{
				indexFound = i;
			}
		}
	}

	if (exists)
	{
		//printf("Error. Variable %c already exists.\n", val);
	}
	else
	{
		if (indexFound >= 0)
		{
			_gVars[indexFound] = val;
			_gValsBack[indexFound] = 0;
			_gVals[indexFound] = num;
			//printf("[Line %d][DEBUG] Created variable [%c] with value of [%d]\n", _gLine, val, num);
		}
		else
		{
			//printf("Error. No space available for variable %c\n", val);
		}
	}
}

void MSEngine::AddCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] -= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] -= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found!\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				//printf("[Line %d][DEBUG] Value %d added to variable %c[%d]\n", _gLine, num, _gVars[index], _gVals[index]);
				_gVals[index] += num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					//printf("[Line %d][DEBUG] Variable %c[%d] added to variable %c[%d]\n", _gLine, _gVars[index2], _gVals[index2], _gVars[index], _gVals[index]);
					_gVals[index] += _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found!\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::SubCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] += num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] += _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				//printf("[Line %d][DEBUG] Value %d subtracted from variable %c[%d]\n", _gLine, num, _gVars[index], _gVals[index]);
				_gVals[index] -= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					//printf("[Line %d][DEBUG] Variable %c[%d] subtracted from variable %c[%d]\n", _gLine, _gVars[index2], _gVals[index2], _gVars[index], _gVals[index]);
					_gVals[index] -= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::MulCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] *= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] *= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				//printf("[Line %d][DEBUG] Value %d multiplied to variable %c[%d]\n", _gLine, num, _gVars[index], _gVals[index]);
				_gVals[index] *= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] *= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("[Line %d][DEBUG][ERROR] Variable [%c] does not exist\n", _gLine, val);
	}
}

void MSEngine::DivCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] /= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] /= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] /= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] /= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::OrrCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] |= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] |= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] |= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] |= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::AndCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] &= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] &= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] &= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] &= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::XorCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] ^= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] ^= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] ^= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] ^= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::EquCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] = -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] = -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] = num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] = _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::BSLCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] <<= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] <<= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] <<= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] <<= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::BSRCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-')
		{
			if (isdigit(_data[startIndex + 4]))
			{
				int32_t num = GetNumber(startIndex + 4, endIndex);
				_gVals[index] >>= -num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 4])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] >>= -_gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 4]);
				}
			}
		}
		else
		{
			if (isdigit(_data[startIndex + 3]))
			{
				int32_t num = GetNumber(startIndex + 3, endIndex);
				_gVals[index] >>= num;
			}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					_gVals[index] >>= _gVals[index2];
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::SwpCommand(int32_t startIndex, int32_t endIndex)
{
	uint8_t val = _data[startIndex];
	bool found = false;
	int32_t index = -1;
	for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
	{
		if (_gVars[i] == val)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (found)
	{
		if (_data[startIndex + 3] == '-') //error
			{
				if (isdigit(_data[startIndex + 4]))
				{
					//int32_t num = GetNumber(startIndex + 4, endIndex);
					//_gVals[index] -= num;
				}
				else
				{
					/*int32_t index2 = -1;
					for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
					{
						if (_gVars[i] == _data[startIndex + 4])
						{
							index = i;
							break;
		}
	}

	if (index2 >= 0)
	{
		int32_t temp = _gVals[index2];
		_gVals[index2] = _gVals[index];
		_gVals[index] = temp;
	}
	else
	{
		printf("Error on line %d: Variable %c not found.", _gLine, _data[startIndex + 4]);
	}*/
				}
			}
		else
		{
			if (isdigit(_data[startIndex + 3])) //error
				{
					//int32_t num = GetNumber(startIndex + 3, endIndex);
					//_gVals[index] += num;
				}
			else
			{
				int32_t index2 = -1;
				for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
				{
					if (_gVars[i] == _data[startIndex + 3])
					{
						index2 = i;
						break;
					}
				}

				if (index2 >= 0)
				{
					int32_t temp = _gVals[index2];
					_gVals[index2] = _gVals[index];
					_gVals[index] = temp;
				}
				else
				{
					//printf("Error on line %d: Variable %c not found.\n", _gLine, _data[startIndex + 3]);
				}
			}
		}
	}
	else
	{
		//printf("Error on line %d: Variable %c not found.\n", _gLine, val);
	}
}

void MSEngine::GPIOCommand(int32_t start, int32_t end)
{
	//g:a:x:124:y
	bool gotGPIO = false;
	bool gotPin = false;
	bool gotState = false;
	bool gotConfig = false;

	uint8_t gpio = 0;
	int32_t pin = 0;
	int32_t state = 0;
	int32_t config = 0;

	int32_t bufIndex = 0;
	uint8_t buf[5];
	for (int32_t j = 0; j < 5; j++)
		buf[j] = '~';
	for (int32_t i = start + 2; i <= end; i++)
	{
		if (_data[i] == ':' || i == end)
		{
			if (!gotGPIO)
			{
				gpio = buf[0];
				gotGPIO = true;
				for (int32_t j = 0; j < 5; j++)
					buf[j] = '~';
				bufIndex = 0;
			}
			else if (!gotPin)
			{
				if (isdigit(buf[0]))
				{
					int32_t len = 0;
					for (int32_t j = 0; j < 5; j++)
					{
						if (buf[j] == '~')
							break;
						len++;
					}
					pin = GetNumber(0, len, buf);
				}
				else
				{
					int32_t index = -1;
					uint8_t c = buf[0];
					for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
					{
						if (_gVars[i] == c)
						{
							index = i;
							break;
						}
					}

					if (index == -1)
					{
						//error
					}
					else
					{
						pin = _gVals[index];
					}
				}
				gotPin = true;
				for (int32_t j = 0; j < 5; j++)
					buf[j] = '~';
				bufIndex = 0;
			}
			else if (!gotState)
			{
				if (isdigit(buf[0]))
				{
					int32_t len = 0;
					for (int32_t j = 0; j < 5; j++)
					{
						if (buf[j] == '~')
							break;
						len++;
					}
					state = GetNumber(0, len, buf);
				}
				else
				{
					int32_t index = -1;
					uint8_t c = buf[0];
					for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
					{
						if (_gVars[i] == c)
						{
							index = i;
							break;
						}
					}

					if (index == -1)
					{
						//error
					}
					else
					{
						state = _gVals[index];
					}
				}
				gotState = true;
				for (int32_t j = 0; j < 5; j++)
					buf[j] = '~';
				bufIndex = 0;
			}
			else if (!gotConfig)
			{
				if (isdigit(buf[0]))
				{
					int32_t len = 0;
					for (int32_t j = 0; j < 5; j++)
					{

						if (buf[j] == '~')
							break;
						len++;
					}
					config = GetNumber(0, len, buf);
				}
				else
				{
					int32_t index = -1;
					uint8_t c = buf[0];
					for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
					{
						if (_gVars[i] == c)
						{
							index = i;
							break;
						}
					}

					if (index == -1)
					{
						//error
					}
					else
					{
						config = _gVals[index];
					}
				}
				gotConfig = true;
				for (int32_t j = 0; j < 5; j++)
					buf[j] = '~';
				bufIndex = 0;
			}
		}
		else
		{
			buf[bufIndex] = _data[i];
			bufIndex++;
		}
	}

	//printf("[Line %d][DEBUG] GPIO Control <=> GPIO[%c], PIN[%d], STATE[%d], CONFIG[%d]\n", _gLine, gpio, pin, state, config);
}

bool MSEngine::IfCommand(int32_t startIndex, int32_t endIndex)
{
	if (_data[startIndex + 2] == '-')
	{

	}
	else
	{
		if (isdigit(_data[startIndex + 2]))
		{

		}
		else
		{
			uint8_t var = _data[startIndex + 2];
			int32_t index = -1;
			for (int32_t i = 0; i < SCRIPT_VAR_SIZE; i++)
			{
				if (_gVars[i] == var)
				{
					index = i;
					break;
				}
			}
			if (index == -1)
			{

			}
			else
			{
				if (_data[startIndex + 3] == '>')
				{
					if (isdigit(_data[startIndex + 4]))
					{
						int32_t x = GetNumber(startIndex + 4, endIndex);
						int32_t y = _gVals[index];
						//printf("[Line %d][DEBUG] If statement %c[%d] > [%d]\n", _gLine, var, y, x);
						if(y > x)
							return true;
						return false;
					}
				}
			}
		}
	}
	return false;
}
