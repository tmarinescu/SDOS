#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "main.hpp"

#define STACK_MAX_SIZE 128
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
	void(*volatile Function)(void);
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
	int32_t ReturnCode;
	Task();
	~Task();
};

class Thread
{
public:
	volatile uint32_t* Stack;
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
	static bool _criticalTaskActive;
	static uint32_t _stackCapture[STACK_MAX_SIZE];
	static Thread _threads[THREAD_NUM];
	static Task _tasks[TASK_NUM];
	static volatile uint32_t __attribute__((section(".ccmram"))) _stack[STACK_MAX_SIZE * THREAD_NUM];
	static uint32_t _stackOffset;
	static uint32_t _threadInitOffset;
	
public:
	static void Update(void);
	static bool EnableThread(ThreadID thread);
	static bool DisableThread(ThreadID thread);
	static bool ResetThread(ThreadID thread);  //Wipes stack and ends active task
	static bool IsThreadEnabled(ThreadID thread);
	static Thread* GetThread(ThreadID thread);
	static bool AssignThreadID(ThreadID thread, uint32_t threadNum, void(*volatile thrd)(ThreadID)); //AssignThreadID(Comms, 0, &Thread1);
	static uint32_t GetActiveTask(ThreadID thread);
	static ThreadID GetActiveThread(void);
	static bool IsThreadIdle(ThreadID thread);
	
	static bool CreateTask(void(*volatile func)(void), PriorityLevel prio, uint32_t* ret_id);
	static bool AddTask(uint32_t id, bool enabled, bool looped);
	static bool RemoveTask(uint32_t id);
	static bool EnableTask(uint32_t id);
	static void DisableTask(uint32_t id);
	static void LoopTask(uint32_t id, uint32_t startDelay, uint32_t delay);
	static void UnloopTask(uint32_t id);
	
	static bool IsValidID(uint32_t id);
	
	static bool InitializeThread(ThreadID thread, ThreadSize size);
	static bool Initialize(void);
	
	/*-Cool Territory- (aka useless but complicated things*/
	
	static ThreadID FindAvailableThread(uint32_t neededStack);  //Find a thread that can immediately execute a small task without dumping old stack or doing any extra work
																//Potentially useful for extremely critical things that need literal immediate response times
	
	static uint32_t UsedStack(ThreadID thread);  //Returns size of how much stack is used at the moment
	static bool SwapStack(ThreadID thread1, ThreadID thread2);  //Not sure if useful, ever, but cool idea
	
	static bool CaptureThread(ThreadID thread);  //Backs up stack contents, only 1 thread at a time, might be useful for real-time thread analysis
};

#endif