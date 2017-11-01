/*
 * sbrk.c
 *
 *  Created on: Aug 26, 2017
 *      Author: technix
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define _COMPILING_NEWLIB // We need newlib internal prototypes.
#include <unistd.h>

#include <errno.h>
#undef errno
extern int errno;

static void *mem_end = NULL;
extern uint8_t __bss_end__;

static inline void *getsp(void)
{
	uint32_t ptr;
	asm volatile ("mov %0, sp" : "=r" (ptr) );
	return (void *)ptr;
}

void *_sbrk(ptrdiff_t incr)
{
	if (!mem_end)
		mem_end = &__bss_end__;

	void *oldbrk = mem_end;
	void *newbrk = oldbrk + incr;

	if (newbrk >= getsp())
	{
		errno = ENOMEM;
		return NULL;
	}

	mem_end = newbrk;
	return oldbrk;
}
