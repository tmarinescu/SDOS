#include "Threads.hpp"

void Thread1(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread2(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread3(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread4(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread5(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread6(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread7(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}

void Thread8(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	int32_t retCode = 0;
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					retCode = (*thrd->AttachedTask->Function)();
					if (thrd->AttachedTask->ReturnHandler != 0)
					{
						(*thrd->AttachedTask->ReturnHandler)(retCode);
					}
					
					if (!thrd->AttachedTask->Loop)
					{
						thrd->AttachedTask->Initialized = false;
					}
					thrd->AttachedTask->AttachedThread = ThreadID::Invalid;
				}
			}
		}
	}
}