/*
 * lcd.h
 *
 *  Created on: Sep 27, 2017
 *      Author: technix
 */

#ifndef INCLUDE_LCD_H_
#define INCLUDE_LCD_H_

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

#define LCD_SIZE_X	16
#define LCD_SIZE_Y	2

enum
{
	IOCTL_LCD_CLEAR,
	IOCTL_LCD_HOME,
	IOCTL_LCD_SET_DIRECTION,
	IOCTL_LCD_SET_STICKY,
	IOCTL_LCD_SET_DISPLAY,
	IOCTL_LCD_SET_CURSOR,
	IOCTL_LCD_SET_BLINK,
	IOCTL_LCD_SHIFT_CURSOR,
	IOCTL_LCD_SHIFT_DISPLAY,
	IOCTL_LCD_SET_CHARACTER
};

enum
{
	LCD_DIRECTION_LEFT = false,
	LCD_DIRECTION_RIGHT = true
};

typedef struct lcd_character
{
	uint8_t character;
	uint8_t bitmap[8];
} lcd_character_t;

__BEGIN_DECLS

int lcd_location(int fd, uint8_t x, uint8_t y);

__END_DECLS

#endif /* INCLUDE_LCD_H_ */
