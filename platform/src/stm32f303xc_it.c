/*
 * stm32f303xc_it.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f303xc_it.h>
#include <system_stm32f3xx.h>
#include <stddef.h>
#include <string.h>

void __stack(void);
void Reset_Handler(void);

void __attribute__((section(".vector"))) (*isr_vector[])(void) =
{
		__stack,
#define HANDLER(name, num) name##_Handler,
#define SKIP_HANDLER NULL,
HANDLER(Reset, 1)
#include <stm32f303xc_handlers.h>
#undef HANDLER
#undef SKIP_HANDLER
};

void __attribute__((noreturn)) _exit(int __attribute__((unused)) value)
{
	// Breakpoint if we are being debugged.
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
		__BKPT(0);

	// Try to reset the system.
	SCB->AIRCR = 0x05fa0000 | SCB_AIRCR_SYSRESETREQ_Msk;

	// Do not return even if we failed to reset.
	for (;;)
		__WFE();
}

extern void *const __vector_load, *const __vector_addr; extern const size_t __vector_size;
extern void *const __ccm_load,    *const __ccm_addr;    extern const size_t __ccm_size;
extern void *const __data_load,   *const __data_addr;   extern const size_t __data_size;
extern void                       *const __bss_addr;    extern const size_t __bss_size;

extern void _start(void);

__attribute__((noreturn, naked)) void Reset_Handler(void)
{
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;
	SCB->CPACR = 0x00f00000;
	__DSB();
	__ISB();

	SystemInit();

	memcpy(__vector_addr, __vector_load, __vector_size);
	memcpy(__ccm_addr, __ccm_load, __ccm_size);
	memcpy(__data_addr, __data_load, __data_size);
	memset(__bss_addr, 0, __bss_size);
	__DSB();

	SCB->VTOR = (uint32_t)isr_vector;
	__DSB();

	SystemCoreClockUpdate();

	_start();
	_exit(0);
}

__attribute__((weak)) void Default_Handler(void)
{
	IRQn_Type IRQn = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) - 16;

	// Breakpoint if we are being debugged.
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
		__BKPT(0);

	if (IRQn < 0)
		_exit(1);
}

#define HANDLER(name, num) void name##_Handler(void) __attribute__((weak, alias("Default_Handler")));
#define SKIP_HANDLER
#include <stm32f303xc_handlers.h>
#undef HANDLER
#undef SKIP_HANDLER
