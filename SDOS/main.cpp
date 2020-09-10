//Static Defined Operating System
//Version 0

//Beware the SRAM size isn't reported correctly because STM provided the incorrect memory map for the G4 and VisualGDB uses that
//AKA the CCMRAM isn't visible, it requires a file modification (this needs to be done every time you update the entire BSP)

/*Leaving this here until it finds its way into a document
(SRAM: 22KB @ 0x2000 0000, CCMRAM: 10KB @ 0x1000 0000)

1. Go to AppData\Local\VisualGDB\EmbeddedBSPs\arm-eabi\com.sysprogs.arm.stm32
2. Modify BSP.XML
3. CTRL+F for "STM32G431KB"
4. Change SRAM size from 32768 to 22528 (Kibibytes)
5. Add this chunk after the MCUMemory of the SRAM; (the address is decimal format, hence why it looks so weird)

	<MCUMemory>
		<Name>CCMRAM</Name>
		<Address>268435456</Address>
		<Size>10240</Size>
		<AddressSpaceSize>0</AddressSpaceSize>
		<Flags>None</Flags>
	</MCUMemory>
	
6. Find RAMSize and change it from 32768 to 22528 (Kibibytes)
7. Restart and re-build and tada;

1>Used FLASH: 10KB out of 128KB (8%)
1>Used SRAM: 16KB out of 22KB (76%)
1>Used CCMRAM: 8192 bytes out of 10240 bytes (80%)

*/

#include "main.hpp"
#include "FPU.hpp"
#include "GPIO.hpp"
#include "Math.hpp"
#include "Float.hpp"
#include "Scheduler.hpp"
#include "System.hpp"
#include "Threads.hpp"

extern "C"
{
	Float x = 0.5f;
	Float y = 25.124f;
	float z = 16.16f;
}

int32_t GPIOTask1()
{
	GPIOBase::Get(8, 'B')->Mode(GPIOMode::OUTPUT_PUSHPULL)->Speed(GPIOSpeed::HIGH)->Pull(GPIOPull::NOPULL)->Update()->Lock()->SetLow();
	return 0;
}

void GPIOTask1_Return(int32_t ret)
{
	if (ret == 0)
	{
		__NOP();  //hoorah
	}
	else
	{
		//bruh
	}
}

int32_t GPIOTask2()
{
	GPIOBase::Get(8, 'B')->Toggle();
	return 0;
}

void GPIOTask2_Return(int32_t ret)
{
	if (ret == 0)
	{
		__NOP();
	}
	else
	{
		
	}
}

int main(void)
{
	//Low level initializers
	FPUnit::Enable();
	FPUnit::SelfTest();
	HAL_Init();
	__disable_irq();
	
	System::SetupSysTick();
	
	//Initialize the scheduler and all the threads/tasks
	Scheduler::Initialize();
	
	//Assign the thread IDs, can mix and match
	
	//Thread 1-4
	Scheduler::AssignThreadID(ThreadID::Comms, 0, &Thread1);
	Scheduler::AssignThreadID(ThreadID::Critical, 1, &Thread2);
	Scheduler::AssignThreadID(ThreadID::Extern, 2, &Thread3);
	Scheduler::AssignThreadID(ThreadID::System, 3, &Thread4);
	
	//Thread 5-8
	Scheduler::AssignThreadID(ThreadID::General1, 4, &Thread5);
	Scheduler::AssignThreadID(ThreadID::General2, 5, &Thread6);
	Scheduler::AssignThreadID(ThreadID::General3, 6, &Thread7);
	Scheduler::AssignThreadID(ThreadID::General4, 7, &Thread8);
	
	//Set their stack size, 256 * 4 (because size of int32 is 4 bytes and there's 256 of them)
	Scheduler::InitializeThread(ThreadID::Comms, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::Critical, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::Extern, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::System, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::General1, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::General2, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::General3, ThreadSize::B256);
	Scheduler::InitializeThread(ThreadID::General4, ThreadSize::B256);
	
	//Enable threads
	Scheduler::EnableThread(ThreadID::Comms);
	Scheduler::EnableThread(ThreadID::Critical);
	Scheduler::EnableThread(ThreadID::Extern);
	Scheduler::EnableThread(ThreadID::System);
	Scheduler::EnableThread(ThreadID::General1);
	Scheduler::EnableThread(ThreadID::General2);
	Scheduler::EnableThread(ThreadID::General3);
	Scheduler::EnableThread(ThreadID::General4);
	
	//Create the tasks
	uint32_t id = 0;
	Scheduler::CreateTask(&GPIOTask1, &GPIOTask1_Return, PriorityLevel::Medium, &id);
	Scheduler::CreateTask(&GPIOTask2, &GPIOTask2_Return, PriorityLevel::Medium, &id, true, 500); //loops every 500ms
	
	SDOS_Setup(); //start kernel
	
	__NOP();
	return 0;
}
