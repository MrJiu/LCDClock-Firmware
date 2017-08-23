/*
 * stm32f303xc_it.h
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#ifndef PLATFORM_INCLUDE_STM32F303XC_IT_H_
#define PLATFORM_INCLUDE_STM32F303XC_IT_H_

#include <stm32f3xx.h>

extern void (*isr_vector[])(void);

#define HANDLER(name, num) void name##_Handler(void);
#define SKIP_HANDLER
#include <stm32f303xc_handlers.h>
#undef HANDLER
#undef SKIP_HANDLER

#endif /* PLATFORM_INCLUDE_STM32F303XC_IT_H_ */
