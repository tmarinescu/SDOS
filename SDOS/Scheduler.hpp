#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "main.hpp"

#define STACK_MAX_SIZE 256
#define THREAD_NUM 8
#define TASK_NUM 256

#define SUPERLOW_QUANTA_CRIT 1
#define SUPERLOW_QUANTA_NORM 2
#define SUPERLOW_FREQ 17000000 //Needs to be checked for validity, can't be a willy nilly frequency

#define LOW_QUANTA_CRIT 2
#define LOW_QUANTA_NORM 5
#define LOW_FREQ 59500000 //Needs to be checked for validity, can't be a willy nilly frequency

#define MEDIUM_QUANTA_CRIT 5
#define MEDIUM_QUANTA_NORM 10
#define MEDIUM_FREQ 85000000 //Needs to be checked for validity, can't be a willy nilly frequency

#define HIGH_QUANTA_CRIT 10
#define HIGH_QUANTA_NORM 20
#define HIGH_FREQ 127500000 //Needs to be checked for validity, can't be a willy nilly frequency

#define CRITICAL_QUANTA 20
#define CRITICAL_FREQ 170000000

extern "C"
{
	void SDOS_Tick(void);
	void SDOS_Setup(void);
	void SDOS_Scheduler(void);
	void SDOS_Pause(void);
	void SDOS_Resume(void);
}

enum TaskError
{
	OutOfMemory,
	Hang,
	SelfCircular,
	None
};

enum ThreadID
{
	Comms = 0,
	System,
	Extern,
	Critical,
	General1,
	General2,
	General3,
	General4,
	Invalid
};

enum ThreadSize
{
	B32,
	B64,
	B128,
	B256,
	B512,
	B1024,
	B2048,
};

enum PriorityLevel
{
	SuperLow,
	Low,
	Medium,
	Normal,
	High,
	Immediate
};

class Task
{
public:
	int32_t (*volatile Function)(void);
	bool Initialized;
	bool Enabled;
	bool Loop;
	bool MemoryWarning;
	bool Blacklisted; //Naughty task
	uint32_t LastExecute; //Current tick
	uint32_t NextExecute; //Current tick + execute time
	uint32_t ExecuteTime; //If looped, execute every 500ms for example
	PriorityLevel Priority;
	ThreadID AttachedThread;
	TaskError LastError;
	uint32_t Quanta;
	uint32_t QuantaMax;
	void(*volatile ReturnHandler)(int32_t ret);
	Task();
	~Task();
};

class Thread
{
public:
	volatile uint32_t* Stack;
	volatile uint32_t StackMax;
	volatile uint32_t StackMin;
	uint32_t Checksum;
	volatile Task* AttachedTask;
	volatile bool Enabled;
	volatile bool Initialized;
	uint32_t Index;
	Thread();
	~Thread();
};

class Scheduler
{
private:
	static volatile bool _running;
	static bool _criticalTaskActive;
	static uint32_t _stackCapture[STACK_MAX_SIZE];
	static bool _capturedThread;
	static ThreadID _capturedThreadID;
	static Thread _threads[THREAD_NUM];
	static Task _tasks[TASK_NUM];
	static volatile uint32_t __attribute__((section(".ccmram"))) _stack[STACK_MAX_SIZE * THREAD_NUM];
	static uint32_t _stackOffset;
	static uint32_t _threadInitOffset;
	static uint32_t _taskCount;
	static volatile uint32_t _tick;
	static volatile uint32_t _curThread;
	static volatile uint32_t _taskIndex;
	static Thread* _activeThread;
	
public:
	static bool ResumeSwitching(void);
	static bool PauseSwitching(void);
	static uint32_t GetTick(void);
	static void Update(void);
	static bool EnableThread(ThreadID thread);
	static bool DisableThread(ThreadID thread);
	static bool ResetThread(ThreadID thread);  //Wipes stack and ends active task
	static bool IsThreadEnabled(ThreadID thread);
	static Thread* GetThread(ThreadID thread);
	static bool AssignThreadID(ThreadID thread, uint32_t threadNum, void(*volatile thrd)(ThreadID)); //AssignThreadID(Comms, 0, &Thread1);
	static Task* GetActiveTask(ThreadID thread);
	static bool IsThreadIdle(ThreadID thread);
	
	static uint32_t DetermineQuanta(PriorityLevel prio);
	static uint32_t DetermineFrequency(PriorityLevel prio);
	
	static bool CreateTask(int32_t(*volatile func)(void), void(*volatile retHandler)(int32_t), PriorityLevel prio, uint32_t* ret_id, bool loop = false, uint32_t delayedStart = 0);
	static bool RemoveTask(uint32_t id);
	static bool EnableTask(uint32_t id);
	static bool DisableTask(uint32_t id);
	static bool LoopTask(uint32_t id, uint32_t startDelay, uint32_t delay);
	static bool UnloopTask(uint32_t id);
	
	static bool IsValidID(uint32_t id);
	
	static bool InitializeThread(ThreadID thread, ThreadSize size);
	static bool Initialize(void);
	
	/*-Cool Territory- (aka useless but complicated things*/
	
	static ThreadID FindAvailableThread(uint32_t neededStack);  //Find a thread that can immediately execute a small task without dumping old stack or doing any extra work
																//Potentially useful for extremely critical things that need literal immediate response times
	
	static uint32_t UsedStack(ThreadID thread);  //Returns size of how much stack is used at the moment
	static bool SwapStack(ThreadID thread1, ThreadID thread2);  //Not sure if useful, ever, but cool idea
	
	static bool CaptureThread(ThreadID thread);  //Backs up stack contents, only 1 thread at a time, might be useful for real-time thread analysis
	static bool ReleaseThread(ThreadID thread);
	
	static uint32_t CalculateChecksum(volatile uint32_t* stackLoc, uint32_t size);
};

#endif