#ifndef _MAIN_H_
#define _MAIN_H_

#define ENABLE_DEBUG_MODE

/*Global includes*/
#include <stm32g4xx_hal.h>
#include <stm32_hal_legacy.h>

/*Extern function shorthands*/
#define cvoid extern "C" void
#define cfloat extern "C" float
#define cuint32_t extern "C" uint32_t

/*Platforms Supported*/
#define PLATFORM_G4
//#define PLATFORM_F7
//#define PLATFORM_F3
//#define PLATFORM_L4

/*Don't mess with*/
#if defined(PLATFORM_G4) || defined(PLATFORM_F3) || defined(PLATFORM_F7) || defined(PLATFORM_L4)


//SysTick
#define SHPR3 (*((volatile uint32_t*)0xE000ED20))

#else
#error "PLATFORM IS NOT DEFINE IN main.hpp"
#endif



#endif
