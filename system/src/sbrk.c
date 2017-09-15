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

extern void *__bss_end;
extern void *__mem_end;

static void *_brk = NULL;

void *_sbrk(ptrdiff_t incr)
{
	if (!_brk)
	{
		_brk = __bss_end;
	}

	void *oldbreak = _brk;
	void *newbreak = _brk + incr;
	if (newbreak > __mem_end)
	{
		errno = ENOMEM;
		return NULL;
	}

	_brk = newbreak;
	return oldbreak;
}
