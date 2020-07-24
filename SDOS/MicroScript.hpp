/*TM*/
/*MicroScript V0.1*/

//Holy mother of unoptimized, this will shred the nearest stack, beware

#ifndef _MICROSCRIPT_H_
#define _MICROSCRIPT_H_

#include "main.hpp"
#include "String.hpp"

#define SCRIPT_MAX_SIZE 256 //Maximum size of script data including terminator
#define SCRIPT_VAR_SIZE 128 //Maximum number of variables declared allowed

class MSEngine
{
public:
	static void Initialize();
	static bool AnalyzeScript(const uint8_t* data);
	static bool AnalyzeScript(String* data);
private:
	static int8_t _data[SCRIPT_MAX_SIZE];
	static int8_t _gVars[SCRIPT_VAR_SIZE];
	static int32_t _gValsBack[SCRIPT_VAR_SIZE];
	static int32_t _gVals[SCRIPT_VAR_SIZE];
	static int8_t _gLine;
	static int32_t _dataLen;
	static int32_t _numSkips;
	static bool ExecuteLine(int32_t startIndex, int32_t endIndex);
	static int32_t GetNumber(int32_t startIndex, int32_t endIndex, uint8_t* data);
	static int32_t GetStartIndexByLine(int32_t line);
	static int32_t GetEndIndexByStartIndex(int32_t index);
	static void WaitCommand(int32_t startIndex, int32_t endIndex);
	static void DebugCommand(int32_t startIndex, int32_t endIndex);
	static void SkipCommand(int32_t startIndex, int32_t endIndex);
	static void ProcessCommand(int32_t startIndex, int32_t endIndex);
	static void ReturnOneCommand(int32_t startIndex, int32_t endIndex);
	static void ReturnAllCommand(int32_t startIndex, int32_t endIndex);
	static void BackupCommand(int32_t startIndex, int32_t endIndex);
	static void LoadCommand(int32_t startIndex, int32_t endIndex);

	static void CreateVariableCommand(int32_t startIndex, int32_t endIndex);
	static void AddCommand(int32_t startIndex, int32_t endIndex);
	static void SubCommand(int32_t startIndex, int32_t endIndex);
	static void MulCommand(int32_t startIndex, int32_t endIndex);
	static void DivCommand(int32_t startIndex, int32_t endIndex);
	static void OrrCommand(int32_t startIndex, int32_t endIndex);
	static void AndCommand(int32_t startIndex, int32_t endIndex);
	static void XorCommand(int32_t startIndex, int32_t endIndex);
	static void EquCommand(int32_t startIndex, int32_t endIndex);
	static void BSLCommand(int32_t startIndex, int32_t endIndex);
	static void BSRCommand(int32_t startIndex, int32_t endIndex);
	static void SwpCommand(int32_t startIndex, int32_t endIndex);

	static void GPIOCommand(int32_t startIndex, int32_t endIndex);
	static bool IfCommand(int32_t startIndex, int32_t endIndex);

	MSEngine();
};

#endif
