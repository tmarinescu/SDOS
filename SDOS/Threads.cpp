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
				}
			}
		}
	}
}