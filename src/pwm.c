/*
 * pwm.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include "pwm.h"

#include <stm32f3xx.h>

__attribute__((constructor(1000))) void pwm_init(void)
{
	// We are only using timer 8 here.
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	RCC->CFGR3 |= RCC_CFGR3_TIM8SW; // 144MHz clock frequency.

	TIM8->CNT = 0;
	TIM8->PSC = 0;
	TIM8->ARR = 0xffff;
	TIM8->RCR = 0;

	TIM8->CCMR1 = 0x68;
	TIM8->CCER = TIM_CCER_CC1E;
	TIM8->BDTR = TIM_BDTR_MOE;
	TIM8->CR1 |= TIM_CR1_CEN;
}

void analogWrite(__attribute__((unused)) uint8_t pin, uint32_t value)
{
	TIM8->CCR1 = value;
}
