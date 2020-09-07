#include "Threads.hpp"

void Thread1(ThreadID thread)
{
	Thread* thrd = Scheduler::GetThread(thread);
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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
	while (true)
	{
		if (thrd->Initialized)
		{
			if (thrd->AttachedTask != 0)
			{
				if (thrd->AttachedTask->Function != 0)
				{
					(*thrd->AttachedTask->Function)();
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