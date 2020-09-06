

# SDOS
Current State: **Not Running**

**Static Defined Operating System**

An operating system that does not deal with dynamic memory allocation due to safety and speed concerns. This system is pre-allocated and pre-defined at compilation.

The HAL API is/will be written from the ground up due to existing API's being poorly structured and written. This API needs to be written for each chip vendor, for now only STM32 will be supported.
<br/>
Implemented HAL APIs:

 - **GPIO** (missing interrupt system)
<br/>

Chip Utilized: **STM32G431KB**
Previous Chip: **STM32F303K8**
Tested Chips: 
 - **STM32F722ZE** (working, TCMRAM issues)
 - **STM32F303RE** (clone of STM32F303K8)
 - **STM32L432KC** (bare-bones working)
 - **STM32H745ZI** (broken, TCCMRAM issues) (dual core is a mess to work with)
<br/>

**OS Layout**

 - Intra-Round Robin w/ Dynamic Priority (quanta time based tasks)
 - 8 Static Threads (user defined amount) (up to 2KB of stack allocation per thread)
 - 256 (meta) Tasks
 - Kernel Security

**Scheduling**

Tasks are executed first-come first-serve with quanta executions. The reason this differs from regular round-robin cycles is that critical tasks follow their own round-robin cycle within the main round-robin [explained in **Critical Tasks**]. Each task gets a certain amount of time to execute (depending on priority) but do not get shifted around as they come in since that is a big performance hit and can cause several blockage issues (several high priority tasks can completely block out lower priority tasks). The idea is that all tasks get to execute regardless of priority but each get different execution times. Once a critical task pops up, all quanta times are halved and the critical task executes immediately interrupting any other tasks (even other critical ones). Again the critical task gets a specific quanta time, if that critical task does not finish, a lower priority task can interrupt it to continue executing. Critical tasks are executed every-other cycle. If there are 8 normal tasks and 1 critical task the 8 tasks will execute round-robin style but the critical task will be pushed between all 8 tasks until it finishes.

**Critical Tasks**

Critical tasks are not allowed to be spam cycled and do not get 100% execution time. If they fail to complete within the desired quanta time, a lower priority task can then take the execution stand. As they come in, they must be executed immediately. If multiple critical tasks are present, they will follow a round-robin of their own every other context switch within the regular round-robin of tasks. The system should not be completely halted from normal operations because a bloated critical task takes place, especially if it is waiting for external response.

**Dynamic Priority**

> *Quanta time is the amount of time a task gets to execute before the context switches to another task*

The quanta time of each task depends on its priority. There are 5 levels of priority (Super Low, Low, Medium, High, Immediate/Critical). Each of these priorities gets different quanta times that can be user defined. As a critical task enters the task pipeline, all existing normal tasks get their quanta times halved. If proven reliable, variable frequency of the CPU will also be implemented into the priority levels.

**Variable CPU Frequency**

Not yet proven or implemented even as a prototype demo. It might or might not prove useful. This system would ramp up/down in frequency of the CPU depending on the priority of the executing tasks however there is a concern in the speed of changing the clock as this might be slow and mess up with the peripherals in the background if they're in suspended states. A separate demo will need to be created to prove if this is even feasible for microcontrollers. In theory it should save power and consume less heat because even at 170MHz (max clock of G4), the chip tends to warm up and if placed within an enclosure it will heat up further.

**Kernel Security**
(Four-layers)
 1. Stack Checksum. A XOR checksum is performed on the stack chunk at context switch for each task and is compared back when resumed, if it differs error out. XOR does not guarantee a unique hash so there is a possibility of different stacks having equal checksums.
 2. Upper (lower) safe-guard limit which brick walls the end of the stack with a hard value, if it changes then error out. The stack can jump over this "wall" and jump back as if nothing happened so it is not foolproof.
 3. Upper and lower stack limit checks. As a context switch happens, the suspended stack location is checked against boundary limits set beforehand, if it is outside the boundary then error out. Again not foolproof as it can only be checked at context switch.
 4. Memory Protection Unit (MPU) is to be utilized if the chip has it. Each stack chunk can be locked temporarily on context switch and error handling is possible (ignore the invalid access and blacklist the task that did it).

**Thread Layout**

 1. Protocol Thread (CAN, I2C, UART)
 2. System Thread (System Management, Timers, Clean-Up)
 3. External Thread (GPIO Input/Output)
 4. Critical (Immediate Tasks Only)
 5. General Work Thread #1
 6. General Work Thread #2
 7. General Work Thread #3
 8. General Work Thread #4
