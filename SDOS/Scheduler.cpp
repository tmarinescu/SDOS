#include  "Scheduler.hpp"
#include "Threads.hpp"

ThreadID ThreadAssignments[THREAD_NUM];
uint32_t ThreadAddresses[THREAD_NUM];

void SDOS_Tick(void)
{
	
}

void SDOS_Setup(void)
{
	
}

void SDOS_Scheduler(void)
{
	
}

bool Scheduler::_criticalTaskActive = false;

uint32_t Scheduler::_stackCapture[STACK_MAX_SIZE];
Thread Scheduler::_threads[THREAD_NUM];
Task Scheduler::_tasks[TASK_NUM];

volatile uint32_t __attribute__((section(".ccmram"))) Scheduler::_stack[STACK_MAX_SIZE * THREAD_NUM];

uint32_t Scheduler::_stackOffset = 0;
uint32_t Scheduler::_threadInitOffset = 0;
void Scheduler::Update(void)
{
	
}

bool Scheduler::EnableThread(ThreadID thread)
{
	
}

bool Scheduler::DisableThread(ThreadID thread)
{
	
}

bool Scheduler::ResetThread(ThreadID thread)
{
	
}

bool Scheduler::IsThreadEnabled(ThreadID thread)
{
	
}

Thread* Scheduler::GetThread(ThreadID thread)
{
	return &_threads[(uint32_t)thread];
}

bool Scheduler::AssignThreadID(ThreadID thread, uint32_t threadNum, void(*volatile thrd)(ThreadID))
{
	if (threadNum >= THREAD_NUM)
		return false;
	
	if (ThreadAssignments[threadNum] != ThreadID::Invalid)
		return false;
	
	ThreadAssignments[threadNum] = thread;
	ThreadAddresses[threadNum] = (uint32_t)thrd;
	
	return true;
}

uint32_t Scheduler::GetActiveTask(ThreadID thread)
{
	
}

ThreadID Scheduler::GetActiveThread(void)
{
	
}

bool Scheduler::IsThreadIdle(ThreadID thread)
{
	
}

	
bool Scheduler::CreateTask(void(*volatile func)(void), PriorityLevel prio, uint32_t* ret_id)
{
	
}

bool Scheduler::AddTask(uint32_t id, bool enabled, bool looped)
{
	
}

bool Scheduler::RemoveTask(uint32_t id)
{
	
}

bool Scheduler::EnableTask(uint32_t id)
{
	
}

void Scheduler::DisableTask(uint32_t id)
{
	
}

void Scheduler::LoopTask(uint32_t id, uint32_t startDelay, uint32_t delay)
{
	
}

void Scheduler::UnloopTask(uint32_t id)
{
	
}

	
bool Scheduler::IsValidID(uint32_t id)
{
	
}

	
bool Scheduler::InitializeThread(ThreadID thread, ThreadSize size)
{
	if (_threadInitOffset >= THREAD_NUM)
		return false;
	
	Thread* thrd = GetThread(thread);
	if (thrd->Initialized)
		return false;
	
	for (uint32_t i = _stackOffset; i < _stackOffset + size; i++)
	{
		_stack[i] = 0;
	}
	
	_stack[_stackOffset + size - 2] = ThreadAddresses[thrd->Index];
	
	thrd->Stack = &_stack[_stackOffset + size - 16]; //Set index to SP
	_stack[_stackOffset + size - 1] = 0x01000000; //xPSR
	
	_stackOffset += size;
	thrd->AttachedTask = 0;
	thrd->Enabled = false;
	thrd->Initialized = true;
	_threadInitOffset++;
	
	return true;
}

bool Scheduler::Initialize(void)
{
	_criticalTaskActive = false;
	_stackOffset = 0;
	_threadInitOffset = 0;
	
	for (uint32_t i = 0; i < STACK_MAX_SIZE * THREAD_NUM; i++)
	{
		_stack[i] = 0;
	}
	for (uint32_t i = 0; i < STACK_MAX_SIZE; i++)
	{
		_stackCapture[i] = 0;
	}
	
	for (uint32_t i = 0; i < THREAD_NUM; i++)
	{
		_threads[i].AttachedTask = 0;
		_threads[i].Enabled = false;
		_threads[i].Stack = 0;
		_threads[i].Initialized = false;
		_threads[i].Index = i;
		ThreadAssignments[i] = ThreadID::Invalid;
		ThreadAddresses[i] = 0;
	}
	
	for (uint32_t i = 0; i < TASK_NUM; i++)
	{
		_tasks[i].AttachedThread = ThreadID::Invalid;
		_tasks[i].Blacklisted = false;
		_tasks[i].Enabled = false;
		_tasks[i].ExecuteTime = 0;
		_tasks[i].Function = 0;
		_tasks[i].LastError = TaskError::None;
		_tasks[i].LastExecute = 0;
		_tasks[i].Loop = false;
		_tasks[i].MemoryWarning = false;
		_tasks[i].NextExecute = 0;
		_tasks[i].Priority = PriorityLevel::SuperLow;
		_tasks[i].Quanta = 0;
		_tasks[i].ReturnCode = 0;
	}
}


	
ThreadID Scheduler::FindAvailableThread(uint32_t neededStack)
{
	
}

	
uint32_t Scheduler::UsedStack(ThreadID thread)
{
	
}

bool Scheduler::SwapStack(ThreadID thread1, ThreadID thread2)
{
	
}

	
bool Scheduler::CaptureThread(ThreadID thread)
{
	
}
