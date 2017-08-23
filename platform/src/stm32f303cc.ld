/*
 * stm32f303cc.ld
 *
 * Linker defining STM32F303CC memory map
 *
 * Copyright (c) 2017 Max Chan. All rights reserved. Licensed under the
 * 3-clause BSD license.
 *
 */
 
MEMORY
{
	IROM (r x) : ORIGIN = 0x08000000, LENGTH = 256k
	IRAM (rw ) : ORIGIN = 0x20000000, LENGTH = 40k
	CCM  (rwx) : ORIGIN = 0x10000000, LENGTH = 8k
}