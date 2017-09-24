/*
 * main.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "board.h"

extern uint32_t __mem_begin, __mem_size;
extern uint32_t __ccmram_size, __ccm_size;

int main(void)
{
	fprintf(stderr, "SushiBits Connected Clock, M205v1. Software version 0.1\r\n");
	fprintf(stderr, "Copyright (c) 2017 Max Chan. All rights reserved.\r\n");
	fprintf(stderr, "Hardware ID: %s <0x%08x>\r\n", board_id, board_hash);
	fprintf(stderr, "%lu bytes stack memory allocated. Total %lu bytes.\r\n", __ccm_size, __ccmram_size);
	fprintf(stderr, "%lu bytes heap memory allocated. Total %lu bytes.\r\n", (uint32_t)sbrk(0) - __mem_begin, __mem_size);
	fprintf(stderr, "Ready.\r\n");

	for (;;)
		__WFE();
}
