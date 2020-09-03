#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "main.hpp"

/*Frequency Manager*/
#define ENABLE_FREQ_MANAGER		true //If enabled manual control over frequency isn't allowed
#define ALLOW_OVERCLOCK			true //Independent of frequency manager (user can manually overclock)
#define ALLOW_THERMAL_THROTTLE	true

/*Boot Settings*/
#define BOOT_RANDOM_SEQUENCE	true //The system boots peripherals in random order (ensures the system starts up correctly in all setting scenarios)
#define BOOT_SRAM_CHECK			true //Validate that the entire SRAM is working at boot
#define BOOT_RANDOM_THREAD		true //Starts off at a random thread, otherwise it defaults to the first
#define BOOT_RANDOM_TASK		true //Starts off at a random task, otherwise it defaults to the first

/*Default Frequencies*/ //according to datasheets
#define G4_DEF_FREQ			16000 //16MHZ
#define F3_DEF_FREQ			8000 //8MHZ

/*Normal Maximum Frequencies*/
#define F3_MAX_FREQ			72000	//72MHZ
#define G4_MAX_FREQ			170000	//170MHZ
#define F7_MAX_FREQ			216000	//216MHZ

/*Frequency Settings For Manager*/
#define G4_OC_FREQ			250000	//250MHZ
#define G4_MIN_FREQ			1000	//1MHZ
#define G4_RAMP_FREQ		1000	//Ramp up/down by 1MHZ
#define G4_RAMP_UP_SPEED	5		//5 milliseconds gradual ramp up
#define G4_RAMP_DOWN_SPEED	2		//2 milliseconds gradual ramp down
#define G4_THERMAL_LIMIT	65		//Maintaing temp around 65 by downclocking

enum CPUClock
{
	FREQ_8MHZ	= 0,
	FREQ_16MHZ	= 1,
	FREQ_36MHZ	= 2,
	FREQ_72MHZ	= 3, 	//F3 max
	FREQ_144MHZ = 4,
	FREQ_170MHZ = 5, 	//G4 max
	FREQ_200MHZ = 6,
	FREQ_216MHZ	= 7, 	//F7 max
	FREQ_250MHZ	= 8,
	FREQ_285MHZ	= 9,
};

class System
{
public:
	static bool SetFrequency(CPUClock freq);
	static CPUClock GetFrequency(void);
	static bool DisableFrequencyManager(void);
	static bool EnableFrequencyManager(void);
	static bool FrequencyManagerEnabled(void);
	static bool HandleFrequencyManager(void);
#ifdef ENABLE_DEBUG_MODE
	static bool StressTestFrequencyManager(void);
#endif
	
	static bool BindBootFunction(void(*func)(void));
	
	static void SetupSysTick(void);
private:
	static bool _frequencyManagerEnabled;
	static CPUClock _currentSetFrequency;
	static uint32_t _currentFrequency;
	static void (*_bootFunction)(void);
};

#endif
