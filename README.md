# SDOS
Static Defined Operating System meant for STM32 microcontrollers.
Current version is written around the STM32G431KB microcontroller.

Proposal:
-heavily modified round robin scheduling with a smart priority system
-8 static threads (or as large as SRAM allows, usually placed in CCMRAM) (estimated 1KByte of stack pre-allocated each)
-up to 256 tasks (immediate, intermediate, cycled)
-heavy kernel safety measures (overflow, underflow, MPU utilization, stack checksum)

1-protocol thread (can, i2c, uart)
2-system thread (timers, management, clean-up)
3-external thread (GPIO input/output)
4-critical (used for immediate tasks)
5-general work thread
6-general work thread
7-general work thread
8-general work thread

Written for Conestoga FSAE
