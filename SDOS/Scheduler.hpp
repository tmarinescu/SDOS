#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

/*
 *  >Stack Safety 4-Method<
 *	 1. Checksum. (A XOR checksum is performed on the stack chunk on switch and compared on resume, if different error out)
 *			i) Recoverable -> Dump Thread -> Blacklist Task -> Continue
 *		
 *	 2. Upper(lower) safeguard limit. (End of stack is brick walled with a hard value, if it changes then error out)
 *			i) Potential permanent stack damage -> Blacklist Task -> System Reset
 *			ii) ***Do note that the stack can jump across the brick wall and back without changing it***
 *		
 *   3. Upper and lower stack limits. (If the actual stack value, whenever possible to be captured, is outside of known boundaries, error out)
 *			i) Potential permanent stack damage -> Blacklist Task -> System Reset
 *			ii) ***Don't always have an eye to the stack value***
 *		
 *   4. Memory Protection Unit. (If the MCU ships with a MPU, use it and lock to the stack chunk)
 *			i) Recoverable -> Blacklist Task -> Continue
 *		
 *	>Scheduling<
 *	 1. Quanta time is how much a task gets to execute before the next task gets its turn.
 *	 
 *	 2. Scheduling is Round Robin with variable quanta times depending on priority.
 *	 
 *	 3. Priority does not mean position manipulation, tasks are executed as they buffer into the pipeline. (except critical tasks)
 *			i) For the above statement, a low priority task can execute before a high priority task, the only difference is quanta time.
 *				
 *	>Critical Handing<
 *	 1. Critical tasks are not allowed to be spam cycled. (No software limit, just don't do it)
 *	 
 *	 2. Critical tasks do not get the priority to execute to 100% completion in one cycle, they only get a higher quanta time and immediately execute.
 *	 
 *	 3. If a critical task takes longer than its quanta time, another task can interrupt for its designated quanta and then resume back to the critical.
 *			i) For the above statement, a critical task will always continue executing every other cycle for its quanta duration.
 *			ii) If more than one critical task is present, they will take turns every other cycle.
 *			iii) The idea here is not to halt the system from doing its normal tasks, just to do them at a much lower effort.
 *				
 *	 4. When a critical task is active, all other priorities get half quanta times.
 *	 
 *	 5. No position manipulation, the scheduler gets warned when a critical task enters the pipeline.
 *	 
 *	 6. When a critical task is created, current executing task suspends and the critical task is executed immediately.
 *	 
 *	 7. Same applies for other critical tasks interrupting other critical tasks, the new ones always execute immediately.
 *	 
 *	 8. The system will ramp up to 100% frequency when a critical task is active.
 * 
 *
 *  >Priority System<
 *	 1. Idea behind frequency variation is for power saving and thermal control. Even at 170MHz, these chips still tend to warm up and if enclosed they will heat up.
 *			i) This system is being designed with multi-agent modularity in mind where multiple chips are each running this OS and communicating with each other.
 *			ii) Frequency variation hasn't been proven yet as a useful concept nor has it been proven to be viable in real-time with active peripherals.
 *	 
 *   2. SuperLow
 *			i) 1ms quanta time (when critical is active)
 *			ii) 2ms quanta time (default)
 *			iii) 10% frequency
 *		
 *   3. Low
 *			i) 2ms quanta time (when critical is active)
 *			ii) 5ms quanta time (default)
 *			iii) 35% frequency
 *		
 *   4. Medium
 *			i) 5ms quanta time (when critical is active)
 *			ii) 10ms quanta time (default)
 *			iii) 50% frequency
 *		
 *   5. High
 *			i) 10ms quanta time (when critical is active)
 *			ii) 20ms quanta time (default)
 *			iii) 75% frequency
 *		
 *   6. Immediate (Critical)
 *			i) 20ms quanta time
 *			ii) 100% frequency
 *  
*/

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