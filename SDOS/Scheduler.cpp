#include  "Scheduler.hpp"
#include "Threads.hpp"
#include "Critical.hpp"

ThreadID ThreadAssignments[THREAD_NUM];
uint32_t ThreadAddresses[THREAD_NUM];

uint32_t Scheduler::_stackOffset = 0;
uint32_t Scheduler::_threadInitOffset = 0;
uint32_t Scheduler::_taskCount = 0;
volatile uint32_t Scheduler::_tick = 0;
volatile uint32_t Scheduler::_curThread = 0;
uint32_t Scheduler::_stackCapture[STACK_MAX_SIZE];
Thread Scheduler::_threads[THREAD_NUM];
Task Scheduler::_tasks[TASK_NUM];

bool Scheduler::_criticalTaskActive = false;
bool Scheduler::_capturedThread = false;
ThreadID Scheduler::_capturedThreadID = ThreadID::Invalid;
Thread* Scheduler::_activeThread = 0;

volatile uint32_t __attribute__((section(".ccmram"))) Scheduler::_stack[STACK_MAX_SIZE * THREAD_NUM];

extern "C"
{
	volatile uint32_t **StackPtr;
	void SDOS_Tick(void)
	{
		HAL_IncTick();
		return;
	}

	void SDOS_Scheduler(void)
	{
		Scheduler::Update();
		return;
	}
}

Thread::Thread()
{
	Stack = 0;
	StackMax = 0;
	StackMin = 0;
	AttachedTask = 0;
	Enabled = false;
	Initialized = false;
	Index = 0;
}

Thread::~Thread()
{
	
}

Task::Task()
{
	Function = 0;
	Initialized = false;
	Enabled = false;
	Loop = false;
	MemoryWarning = false;
	Blacklisted = false;
	LastExecute = 0;
	NextExecute = 0;
	ExecuteTime = 0;
	Priority = PriorityLevel::Low;
	AttachedThread = ThreadID::Invalid;
	LastError = TaskError::None;
	Quanta = 0;
	QuantaMax = 0;
	ReturnHandler = 0;
}

Task::~Task()
{
	
}

uint32_t Scheduler::GetTick(void)
{
	return _tick;
}

void Scheduler::Update(void)
{
	uint32_t prim = Critical::DisableAllInterrupts();
	_curThread++;
	if (_curThread >= THREAD_NUM)
		_curThread = 0;
	
	while (!_threads[_curThread].Enabled)
	{
		//Deadlock warning
		_curThread++;
		if (_curThread >= THREAD_NUM)
			_curThread = 0;
	}
	
	StackPtr = &_threads[_curThread].Stack;
	_tick++;
	Critical::EnableAllInterrupts(prim);
}

bool Scheduler::EnableThread(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return false;
	
	if (!thrd->Initialized)
		return false;
	
	if (thrd->Enabled)
		return false;
	
	thrd->Enabled = true;
	
	return true;
}

bool Scheduler::DisableThread(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return false;
	
	if (!thrd->Initialized)
		return false;
	
	if (!thrd->Enabled)
		return false;
	
	thrd->Enabled = false;
	
	return true;
}

bool Scheduler::ResetThread(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return false;
	
	if (!thrd->Initialized)
		return false;
	
	thrd->Initialized = false;
	
	return true;
}

bool Scheduler::IsThreadEnabled(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return false;
	
	if (!thrd->Initialized)
		return false;
	
	return thrd->Enabled;
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
	
	Thread* thr = GetThread(thread);
	if (thr != 0)
		thr->Index = threadNum;
	
	return true;
}

Task* Scheduler::GetActiveTask(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return 0;
	
	if (!thrd->Initialized)
		return 0;
	
	return thrd->AttachedTask;
}

bool Scheduler::IsThreadIdle(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
		return true;
	
	if (!thrd->Initialized)
		return true;
	
	if (!thrd->Enabled)
		return true;
	
	return thrd->Index == _curThread;
}

uint32_t Scheduler::DetermineFrequency(PriorityLevel prio)
{
	if (prio == PriorityLevel::SuperLow)
	{
		return SUPERLOW_FREQ;
	}
	else if (prio == PriorityLevel::Low)
	{
		return LOW_FREQ;
	}
	else if (prio == PriorityLevel::Medium)
	{
		return MEDIUM_FREQ;
	}
	else if (prio == PriorityLevel::High)
	{
		return HIGH_FREQ;
	}
	else
	{
		return CRITICAL_FREQ;
	}
}

uint32_t Scheduler::DetermineQuanta(PriorityLevel prio)
{
	if (prio == PriorityLevel::SuperLow)
	{
		if (_criticalTaskActive)
		{
			return SUPERLOW_QUANTA_CRIT;
		}
		else
		{
			return SUPERLOW_QUANTA_NORM;
		}
	}
	else if (prio == PriorityLevel::Low)
	{
		if (_criticalTaskActive)
		{
			return LOW_QUANTA_CRIT;
		}
		else
		{
			return LOW_QUANTA_NORM;
		}
	}
	else if (prio == PriorityLevel::Medium)
	{
		if (_criticalTaskActive)
		{
			return MEDIUM_QUANTA_CRIT;
		}
		else
		{
			return MEDIUM_QUANTA_NORM;
		}
	}
	else if (prio == PriorityLevel::High)
	{
		if (_criticalTaskActive)
		{
			return HIGH_QUANTA_CRIT;
		}
		else
		{
			return HIGH_QUANTA_NORM;
		}
	}
	else
	{
		return CRITICAL_QUANTA;
	}
}
	
bool Scheduler::CreateTask(int32_t(*volatile func)(void), void(*volatile retHandler)(int32_t), PriorityLevel prio, uint32_t* ret_id, bool loop, uint32_t delayedStart)
{
	uint32_t prim = Critical::DisableAllInterrupts();
	uint32_t indx = 0;
	for (indx = 0; indx < TASK_NUM; indx++)
	{
		if (_tasks[indx].Initialized)
		{
			continue;
		}
		else
		{
			_tasks[indx].Initialized = true;
			_tasks[indx].Blacklisted = false;
			_tasks[indx].Enabled = false;
			_tasks[indx].ExecuteTime = delayedStart;
			_tasks[indx].Function = func;
			_tasks[indx].LastError = TaskError::None;
			_tasks[indx].LastExecute = GetTick();
			_tasks[indx].Loop = loop;
			_tasks[indx].MemoryWarning = false;
			_tasks[indx].NextExecute = delayedStart + GetTick();
			_tasks[indx].Priority = prio;
			_tasks[indx].Quanta = 0;   //Determined within scheduler update
			_tasks[indx].QuantaMax = 0;
			_tasks[indx].ReturnHandler = retHandler;
			_taskCount++;
			*ret_id = indx;
			Critical::EnableAllInterrupts(prim);
			return true;
		}
	}
	
	ret_id = 0;
	Critical::EnableAllInterrupts(prim);
	return false;
}

bool Scheduler::RemoveTask(uint32_t id)
{
	if (id >= TASK_NUM)
		return false;
	
	_tasks[id].Initialized = false;
	return true;
}

bool Scheduler::EnableTask(uint32_t id)
{
	if (id >= TASK_NUM)
		return false;
	
	if (!_tasks[id].Initialized)
		return false;
	
	if (_tasks[id].Enabled)
		return false;
	
	_tasks[id].Enabled = true;
	return true;
}

bool Scheduler::DisableTask(uint32_t id)
{
	if (id >= TASK_NUM)
		return false;
	
	if (!_tasks[id].Initialized)
		return false;
	
	if (!_tasks[id].Enabled)
		return false;
	
	_tasks[id].Enabled = false;
	return true;
}

bool Scheduler::LoopTask(uint32_t id, uint32_t startDelay, uint32_t delay)
{
	if (id >= TASK_NUM)
		return false;
	
	if (!_tasks[id].Initialized)
		return false;
	
	if (_tasks[id].Loop)
		return false;
	
	_tasks[id].Loop = true;
	return true;
}

bool Scheduler::UnloopTask(uint32_t id)
{
	if (id >= TASK_NUM)
		return false;
	
	if (!_tasks[id].Initialized)
		return false;
	
	if (!_tasks[id].Loop)
		return false;
	
	_tasks[id].Loop = false;
	return true;
}

	
bool Scheduler::IsValidID(uint32_t id)
{
	if (id >= TASK_NUM)
		return false;
	
	return _tasks[id].Initialized;
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
	
	thrd->StackMin = _stackOffset;
	thrd->StackMax = _stackOffset + size;
	_stack[_stackOffset + size - 2] = ThreadAddresses[thrd->Index]; //PC
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
	_capturedThread = false;
	_capturedThreadID = ThreadID::Invalid;
	
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
		_threads[i].Index = 0;
		_threads[i].StackMax = 0;
		_threads[i].StackMin = 0;
		ThreadAssignments[i] = ThreadID::Invalid;
		ThreadAddresses[i] = 0;
	}
	
	for (uint32_t i = 0; i < TASK_NUM; i++)
	{
		_tasks[i].Initialized = false;
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
		_tasks[i].QuantaMax = 0;
		_tasks[i].ReturnHandler = 0;
	}
	
	return true;
}


	
ThreadID Scheduler::FindAvailableThread(uint32_t neededStack)
{
	for (uint32_t i = 0; i < THREAD_NUM; i++)
	{
		if (_threads[i].Initialized)
		{
			if (*_threads[i].Stack + neededStack < _threads[i].StackMax)
				return (ThreadID)i;
		}
	}
	
	return ThreadID::Invalid;
}

	
uint32_t Scheduler::UsedStack(ThreadID thread)
{
	Thread* thrd = GetThread(thread);
	if (thrd != 0)
	{
		if (thrd->Initialized)
		{
			return *thrd->Stack - thrd->StackMin;
		}
	}
	
	return 0;
}

bool Scheduler::SwapStack(ThreadID thread1, ThreadID thread2)
{
	uint32_t prim = Critical::DisableAllInterrupts();
	
	Thread* thrd1 = GetThread(thread1);
	if (thrd1 == 0)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	if (!thrd1->Initialized)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	Thread* thrd2 = GetThread(thread2);
	if (thrd2 == 0)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	if (!thrd2->Initialized)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	volatile uint32_t* temp = thrd1->Stack;
	thrd1->Stack = thrd2->Stack;
	thrd2->Stack = temp;
	
	Critical::EnableAllInterrupts(prim);
	return true;
}

	
bool Scheduler::CaptureThread(ThreadID thread)
{
	uint32_t prim = Critical::DisableAllInterrupts();
	if (_capturedThread)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	Thread* thrd = GetThread(thread);
	if (thrd == 0)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	if (!thrd->Initialized)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	uint32_t j = 0;
	for (uint32_t i = thrd->StackMin; i < thrd->StackMax; i++)
	{
		_stackCapture[j] = _stack[i];
		j++;
	}
	
	_capturedThread = true;
	_capturedThreadID = thread;
	Critical::EnableAllInterrupts(prim);
	return true;
}

bool Scheduler::ReleaseThread(ThreadID thread)
{
	uint32_t prim = Critical::DisableAllInterrupts();
	
	if (!_capturedThread)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	if (thread != _capturedThreadID)
	{
		Critical::EnableAllInterrupts(prim);
		return false;
	}
	
	for (uint32_t i = 0; i < STACK_MAX_SIZE; i++)
	{
		_stackCapture[i] = 0;
	}
	
	_capturedThread = false;
	_capturedThreadID = ThreadID::Invalid;
	
	Critical::EnableAllInterrupts(prim);
	return false;
}
