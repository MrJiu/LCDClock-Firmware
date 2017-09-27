/*
 * lcd.c
 *
 *  Created on: Sep 27, 2017
 *      Author: technix
 */

#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <dreamos-rt/ring-buffer.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <dreamos-rt/device.h>
#include <dreamos-rt/time.h>
#include <dreamos-rt/gpio.h>
#include <stm32f303xc_it.h>
#include <stm32f3xx.h>
#include <unistd.h>

#include <errno.h>
#undef errno
extern int errno;

enum
{
	LCD_FLAG_DIRECTION	= 0x0001,
	LCD_FLAG_STICKY		= 0x0002,
	LCD_FLAG_DISPLAY		= 0x0004,
	LCD_FLAG_CURSOR		= 0x0008,
	LCD_FLAG_BLINK		= 0x0010
};

typedef struct lcd_s lcd_t;
struct lcd_s
{
	device_t device;

	uint8_t flags;
	uint8_t idx;
};

static void lcd_write_value(lcd_t *lcd, uint8_t addr, uint8_t value)
{
	digitalWrite(0x10, addr ? true : false);
	digitalWrite(0x12, false);
	SET_FIELD(GPIOA->MODER, 0xffff, 0x5555);
	SET_FIELD(GPIOA->ODR, 0xff, value);
	usleep(1);
	digitalWrite(0x1d, true);
	usleep(2);
	digitalWrite(0x1d, false);
	usleep(40);
}

static uint8_t lcd_read_value(lcd_t *lcd, uint8_t addr)
{
	digitalWrite(0x10, addr ? true : false);
	digitalWrite(0x12, true);
	SET_FIELD(GPIOA->MODER, 0xffff, 0x0000);
	usleep(1);
	digitalWrite(0x1d, true);
	usleep(1);
	uint8_t val = GPIOA->IDR & 0xff;
	usleep(1);
	digitalWrite(0x1d, false);
	usleep(40);
	SET_FIELD(GPIOA->MODER, 0xffff, 0x5555);
	return val;
}

static inline uint8_t lcd_get_device_address(int8_t offset)
{
	uint8_t lineaddr = offset % 80;
	return ((offset > 40) ? 0x40 : 0) + lineaddr;
}

static int lcd_open(device_t *device, int mode, ...)
{
	lcd_t *lcd = (lcd_t *)device;

	lcd_write_value(lcd, false, 0x38);
	usleep(4500);
	lcd_write_value(lcd, false, 0x38);
	usleep(100);
	lcd_write_value(lcd, false, 0x38);
	lcd_write_value(lcd, false, 0x01);
	usleep(1500);
	lcd_write_value(lcd, false, 0x06);
	lcd_write_value(lcd, false, 0x0d);

	lcd->flags = LCD_FLAG_DIRECTION | LCD_FLAG_DISPLAY | LCD_FLAG_BLINK;
	lcd->idx = 0;

	return 0;
}

static int lcd_close(device_t *device)
{
	lcd_t *lcd = (lcd_t *)device;

	lcd_write_value(lcd, false, 0x09);
	lcd->flags &= ~LCD_FLAG_DISPLAY;

	return 0;
}

static int lcd_read(device_t *device, void *buf, size_t len)
{
	lcd_t *lcd = (lcd_t *)device;
	char *bp = buf;
	int count = 0;

	for (int idx = 0; idx < len; idx++)
	{
		bp[idx] = lcd_read_value(lcd, true);
		lcd->idx = (lcd->idx + 1) % 80;
		count++;
	}

	return count;
}

static int lcd_write(device_t *device, const void *buf, size_t len)
{
	lcd_t *lcd = (lcd_t *)device;
	const char *bp = buf;
	int count = 0;

	for (int idx = 0; idx < len; idx++)
	{
		lcd_write_value(lcd, true, bp[idx]);
		lcd->idx = (lcd->idx + 1) % 80;
		count++;
	}

	return count;
}

static int lcd_ioctl(device_t *device, unsigned long func, ...)
{
	lcd_t *lcd = (lcd_t *)device;

	uint32_t value;
	va_list args;
	va_start(args, func);
	value = va_arg(args, uint32_t);
	va_end(args);

	switch (func)
	{
	case IOCTL_LCD_CLEAR:
		lcd_write_value(lcd, false, 0x01);
		usleep(1500);
		return 0;

	case IOCTL_LCD_HOME:
		lcd_write_value(lcd, false, 0x02);
		return 0;

	case IOCTL_LCD_SET_DIRECTION:
		SET_FIELD(lcd->flags, LCD_FLAG_DIRECTION, value ? LCD_FLAG_DIRECTION : 0);
		lcd_write_value(lcd, false,
				0x04 |
				((lcd->flags & LCD_FLAG_DIRECTION) ? 0x02 : 0) |
				((lcd->flags & LCD_FLAG_STICKY) ? 0x01 : 0));
		return 0;

	case IOCTL_LCD_SET_STICKY:
		SET_FIELD(lcd->flags, LCD_FLAG_STICKY, value ? LCD_FLAG_STICKY : 0);
		lcd_write_value(lcd, false,
				0x04 |
				((lcd->flags & LCD_FLAG_DIRECTION) ? 0x02 : 0) |
				((lcd->flags & LCD_FLAG_STICKY) ? 0x01 : 0));
		return 0;

	case IOCTL_LCD_SET_DISPLAY:
		SET_FIELD(lcd->flags, LCD_FLAG_DISPLAY, value ? LCD_FLAG_DISPLAY : 0);
		lcd_write_value(lcd, false,
				0x08 |
				((lcd->flags & LCD_FLAG_DISPLAY) ? 0x04 : 0) |
				((lcd->flags & LCD_FLAG_CURSOR) ? 0x02 : 0) |
				((lcd->flags & LCD_FLAG_BLINK) ? 0x01 : 0));
		return 0;

	case IOCTL_LCD_SET_CURSOR:
		SET_FIELD(lcd->flags, LCD_FLAG_CURSOR, value ? LCD_FLAG_CURSOR : 0);
		lcd_write_value(lcd, false,
				0x08 |
				((lcd->flags & LCD_FLAG_DISPLAY) ? 0x04 : 0) |
				((lcd->flags & LCD_FLAG_CURSOR) ? 0x02 : 0) |
				((lcd->flags & LCD_FLAG_BLINK) ? 0x01 : 0));
		return 0;

	case IOCTL_LCD_SET_BLINK:
		SET_FIELD(lcd->flags, LCD_FLAG_BLINK, value ? LCD_FLAG_BLINK : 0);
		lcd_write_value(lcd, false,
				0x08 |
				((lcd->flags & LCD_FLAG_DISPLAY) ? 0x04 : 0) |
				((lcd->flags & LCD_FLAG_CURSOR) ? 0x02 : 0) |
				((lcd->flags & LCD_FLAG_BLINK) ? 0x01 : 0));
		return 0;

	case IOCTL_LCD_SHIFT_CURSOR:
		lcd_write_value(lcd, false, 0x10 | value ? 0x4 : 0);
		return 0;

	case IOCTL_LCD_SHIFT_DISPLAY:
		lcd_write_value(lcd, false, 0x18 | value ? 0x4 : 0);
		return 0;

	case IOCTL_LCD_SET_CHARACTER:
	{
		lcd_character_t ch = (lcd_character_t)value;
		uint8_t charaddr = (ch->character & 0x7) << 3;
		lcd_write_value(lcd, false, 0x40 | charaddr);
		for (int idx = 0; idx < 8; idx++)
			lcd_write_value(lcd, true, ch->bitmap[idx]);
		lcd_write_value(lcd, false, 0x80 | lcd_get_device_address(lcd->idx));
		return 0;
	}

	default:
		errno = EINVAL;
		return -1;
	}
}

static off_t lcd_lseek(device_t *device, off_t offset, int whence)
{
	lcd_t *lcd = (lcd_t *)device;

	switch (whence)
	{
	case SEEK_SET:
		lcd->idx = offset;
		break;

	case SEEK_CUR:
		lcd->idx += offset;
		break;

	default:
		errno = EINVAL;
		return -1;
	}

	lcd->idx %= 80;
	lcd_write_value(lcd, false, 0x80 | lcd_get_device_address(lcd->idx));

	return lcd->idx;
}

int lcd_location(int fd, uint8_t x, uint8_t y)
{
	return lseek(fd, y * 40 + x, SEEK_SET);
}

static int lcd_fstat(device_t *device, struct stat *st)
{
	if (st)
	{
		st->st_dev = 0;
		st->st_ino = 0;
		st->st_mode = 0644;
		st->st_nlink = 1;
		st->st_uid = 0;
		st->st_gid = 0;
		st->st_rdev = 1;
		st->st_size = 0;
		st->st_atime = 0;
		st->st_mtime = 0;
		st->st_ctime = 0;
		st->st_blksize = 0;
		st->st_blocks = 0;
	}
	return 0;
}

static int lcd_isatty(device_t *device)
{
	return 0;
}

static const driver_t lcd_driver =
{
		.name = "hd44780-lcd1602-driver",
		.open = lcd_open,
		.close = lcd_close,
		.read = lcd_read,
		.write = lcd_write,
		.ioctl = lcd_ioctl,
		.fstat = lcd_fstat,
		.lseek = lcd_lseek,
		.isatty = lcd_isatty
};

__attribute__((section(".ccm"))) static lcd_t lcd =
{
		{
				.name = "lcd",
				.driver = &lcd_driver
		}
};
__attribute__((section(".devices"))) const lcd_t *LCD = &lcd;
