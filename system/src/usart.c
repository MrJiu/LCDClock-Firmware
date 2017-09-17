/*
 * usart.c
 *
 *  Created on: Aug 26, 2017
 *      Author: technix
 */

#include <dreamos-rt/usart.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <dreamos-rt/ring-buffer.h>

#include <errno.h>
#undef errno
extern int errno;

typedef struct usart_s usart_t;

struct usart_s
{
	// Static data.
	device_t device;
	USART_TypeDef *const USART;
	IRQn_Type IRQn;
	volatile uint32_t *const RCC_APBEN;
	const uint32_t RCC_APBEN_Msk;

	// Dynamic data
	int open_mode;

	ring_buffer_t read_buffer;
	ring_buffer_t write_buffer;
};

static void usart_interrupt(usart_t *usart)
{
	;
}

static inline void usart_wait_for_write(usart_t *usart)
{
	while (ring_buffer_getlength(usart->write_buffer));
	while (!(usart->USART->ISR & USART_ISR_TC));
}

static int usart_open(device_t *device, int mode, ...)
{
	usart_t *usart = (usart_t *)device;

	if (!usart->read_buffer)
	{
		usart->read_buffer = ring_buffer_init(USART_READ_BUFFER_SIZE);
		if (!usart->read_buffer)
			return -1;
	}

	if (!usart->write_buffer)
	{
		usart->write_buffer = ring_buffer_init(USART_WRITE_BUFFER_SIZE);
		if (!usart->write_buffer)
		{
			ring_buffer_dealloc(usart->read_buffer);
			usart->read_buffer = NULL;
			return -1;
		}
	}

	(*usart->RCC_APBEN) |= usart->RCC_APBEN_Msk;

	SET_FIELD(usart->USART->CR1, USART_CR1_M0, 0);					// 8-N-1
	SET_FIELD(usart->USART->CR2, USART_CR2_STOP, 0);
	usart->USART->BRR = SystemCoreClock / USART_DEFAULT_BAUDRATE;	// 115200 baud

	// Enable interrupts
	NVIC_EnableIRQ(usart->IRQn);

	usart->USART->CR1 |= USART_CR1_UE;
	__DMB();
	usart->USART->CR1 |= USART_CR1_TE | USART_CR1_RE;

	usart->open_mode = mode;

	return 0;
}

static int usart_close(device_t *device)
{
	usart_t *usart = (usart_t *)device;

	// Gracefully shut down USART
	while (usart->USART->ISR & USART_ISR_BUSY);
	usart->USART->CR1 &= ~USART_CR1_RE;
	usart_wait_for_write(usart);
	usart->USART->CR1 &= ~USART_CR1_TE;
	usart->USART->CR1 &= ~USART_CR1_UE;

	// Disable interrupts.
	NVIC_DisableIRQ(usart->IRQn);

	(*usart->RCC_APBEN) &= ~usart->RCC_APBEN_Msk;

	ring_buffer_dealloc(usart->read_buffer);
	ring_buffer_dealloc(usart->write_buffer);
	usart->read_buffer = NULL;
	usart->write_buffer = NULL;

	return 0;
}

static inline int usart_getchar_nonblock(usart_t *usart)
{

}

static inline int usart_putchar_nonblock(usart_t *usart, char ch)
{

}

static int usart_read(device_t *device, void *buf, size_t len)
{
	usart_t *usart = (usart_t *)device;
	char *bp = buf;
	int count = 0;

	for (int idx = 0; idx < len; idx++)
	{
		int ch = usart_getchar_nonblock(usart);
		if (ch < 0)
		{
			errno = -ch;
			break;
		}
		bp[idx] = ch;
	}

	return count;
}

static int usart_write(device_t *device, const void *buf, size_t len)
{
	usart_t *usart = (usart_t *)device;
	const char *bp = buf;
	int count = 0;
	return -1;
}

static int usart_ioctl(device_t *device, unsigned long func, ...)
{
	usart_t *usart = (usart_t *)device;

	uint32_t value;
	va_list args;
	va_start(args, func);
	value = va_arg(args, uint32_t);
	va_end(args);

	switch (func)
	{
	case IOCTL_USART_GET_BAUDRATE:
		return SystemCoreClock / usart->USART->BRR;

	case IOCTL_USART_SET_BAUDRATE:
		while (usart->USART->ISR & USART_ISR_BUSY);
		usart->USART->CR1 &= ~USART_CR1_RE;
		usart_wait_for_write(usart);
		usart->USART->CR1 &= ~USART_CR1_TE;
		usart->USART->CR1 &= ~USART_CR1_UE;
		__DMB();

		usart->USART->BRR = SystemCoreClock / value;
		__DMB();

		usart->USART->CR1 |= USART_CR1_UE;
		usart->USART->CR1 |= USART_CR1_TE | USART_CR1_RE;

		return 0;

	default:
		errno = EINVAL;
		return -1;
	}
}

static int usart_poll(device_t *device, short events, short *revents)
{
	usart_t *usart = (usart_t *)device;
	return -1;
}

static const driver_t usart_driver;

static int usart_fstat(device_t *device, struct stat *st)
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

static int usart_isatty(device_t *device)
{
	return 1;
}

static const driver_t usart_driver =
{
		"stm32f3xx-usart",
		NULL,
		usart_open,
		usart_close,
		usart_read,
		usart_write,
		usart_ioctl,
		usart_poll,
		usart_fstat,
		NULL,
		usart_isatty
};

__attribute__((section(".ccm"))) static usart_t tty0 =
{
		{
				&usart_driver,
				"tty0",
		},
		USART1,
		USART1_IRQn,
		&(RCC->APB2ENR),
		RCC_APB2ENR_USART1EN
};
__attribute__((section(".devices"))) const usart_t *TTY0 = &tty0;
void USART1_Handler(void) { usart_interrupt(&tty0); }

__attribute__((section(".ccm"))) static usart_t tty1 =
{
		{
				&usart_driver,
				"tty1",
		},
		USART2,
		USART2_IRQn,
		&(RCC->APB1ENR),
		RCC_APB1ENR_USART2EN
};
__attribute__((section(".devices"))) const usart_t *TTY1 = &tty1;
void USART2_Handler(void) { usart_interrupt(&tty1); }

__attribute__((section(".ccm"))) static usart_t tty2 =
{
		{
				&usart_driver,
				"tty2",
		},
		USART3,
		USART3_IRQn,
		&(RCC->APB1ENR),
		RCC_APB1ENR_USART3EN
};
__attribute__((section(".devices"))) const usart_t *TTY2 = &tty2;
void USART3_Handler(void) { usart_interrupt(&tty2); }

/*
__attribute__((section(".ccm"))) static usart_t tty3 =
{
		{
				&usart_driver,
				"tty3",
		},
		UART4,
		UART4_IRQn,
		&(RCC->APB1ENR),
		RCC_APB1ENR_UART4EN
};
__attribute__((section(".devices"))) const usart_t *TTY3 = &tty3;
void UART4_Handler(void) { usart_interrupt(&tty3); }

__attribute__((section(".ccm"))) static usart_t tty4 =
{
		{
				&usart_driver,
				"tty4",
		},
		UART5,
		UART5_IRQn,
		&(RCC->APB1ENR),
		RCC_APB1ENR_UART5EN
};
__attribute__((section(".devices"))) const usart_t *TTY4 = &tty4;
void UART5_Handler(void) { usart_interrupt(&tty4); }
*/
