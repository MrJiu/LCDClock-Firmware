/*
 * board.h
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include <sys/cdefs.h>
#include <stdint.h>

__BEGIN_DECLS

#define BOARD_ID		((uint8_t *)0x1ffff7ac)

extern const uint8_t board_id[36];
extern const uint32_t board_hash;

__END_DECLS

#endif /* INCLUDE_BOARD_H_ */
