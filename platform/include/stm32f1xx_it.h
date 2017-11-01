/*
 * stm32f1xx_it.h
 *
 *  Created on: Oct 29, 2017
 *      Author: technix
 */

#ifndef PLATFORM_INCLUDE_STM32F1XX_IT_H_
#define PLATFORM_INCLUDE_STM32F1XX_IT_H_

extern void (*isr_vector[])(void);

#define HANDLER(name, value) void name ##_IRQHandler(void);
#define SKIP_HANDLER
#include <stm32f1xx_handler.h>
#undef HANDLER
#undef SKIP_HANDLER

#endif /* PLATFORM_INCLUDE_STM32F1XX_IT_H_ */
