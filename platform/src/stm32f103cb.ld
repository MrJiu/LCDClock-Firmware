/*
 * cortex-m.ld
 *
 * Common linker script for Cortex-M with two memory blocks.
 *
 * Copyright (c) 2017 Max Chan. All rights reserved. Licensed under the
 * 3-clause BSD license.
 *
 */

/*
 * Memory ranges:
 *
 *    IROM: Internal Flash.
 *    IRAM: Internal RAM, used for heap, allocations and DMA.
 */
 
 MEMORY
{
	IROM (r x) : ORIGIN = 0x08000000, LENGTH = 256k
	IRAM (rw ) : ORIGIN = 0x20000000, LENGTH = 40k
}

/* On this runtime environment, the function Reset_Handler is our entry point,
 * instead of the usual _start. This function prepares the clocks, and copies
 * data and CCM code sections in place, before calling _start like usual.
 */
ENTRY(Reset_IRQHandler)
EXTERN(isr_vector)

SECTIONS
{
	. = ORIGIN(IROM);
	
	/* We place the interrupt vector at the beginning of CCM so things can be
	 * faster when interrupts are being fired.
	 */
	.vector :
	{
		PROVIDE(__vector_start = .);
		KEEP(*(.vector))
		ASSERT(. != __vector_start, "The interrupt vector is missing.");
		. = ALIGN(4);
	} > CCM AT > IROM
	
	/* Now comes the bulk of the application code in Flash. */
	.text :
	{
		*(.text*)
	} > IROM
	
	.devices :
	{
		KEEP(*(.devices*))
	} > IROM
	
	.rodata :
	{
		*(.rodata*)
		
		/* Address tables */
		PROVIDE(__devices_addr = .); LONG(ADDR(.devices));
		PROVIDE(__devices_size = .); LONG(SIZEOF(.devices));
		PROVIDE(__copy_addr = .); LONG(ADDR(.copytable));
		PROVIDE(__copy_size = .); LONG(SIZEOF(.copytable));
		PROVIDE(__zero_addr = .); LONG(ADDR(.zerotable));
		PROVIDE(__zero_size = .); LONG(SIZEOF(.zerotable));
		
		PROVIDE(__mem_begin = .); LONG(ORIGIN(IRAM));
		PROVIDE(__mem_size = .); LONG(LENGTH(IRAM));
		PROVIDE(__bss_end = .); LONG(ADDR(.bss) + SIZEOF(.bss));
		PROVIDE(__mem_end = .); LONG(ORIGIN(IRAM) + LENGTH(IRAM));
	} > IROM
	
	.copytable :
	{
		LONG(LOADADDR(.vector));
		LONG(ADDR(.vector));
		LONG(SIZEOF(.vector));
		
		LONG(LOADADDR(.data));
		LONG(ADDR(.data));
		LONG(SIZEOF(.data));
	} > IROM
	
	.zerotable :
	{
		LONG(ADDR(.bss));
		LONG(SIZEOF(.bss));
	} > IROM
	
	.init :
	{
		KEEP(*(.init))
	} > IROM
	
	.fini :
	{
		KEEP(*(.fini))
	} > IROM
	
    /*
     * Stack unwinding and exception handling sections.
     *
     * ARM compilers emit object files with .ARM.extab and .ARM.exidx sections
     * when using C++ exceptions. Also, at least GCC emits those sections when
     * dividing large numbers (64-bit) in C. So we have to handle them.
     *
     * (ARM uses .ARM.extab and .ARM.exidx instead of the .eh_frame section
     * used on x86.)
     */
    .ARM.extab : /* exception unwinding information */
    {
        *(.ARM.extab*)
    } > IROM
    
    .ARM.exidx : /* index entries for section unwinding */
    {
        *(.ARM.exidx*)
    } > IROM

    /*
     * Newlib and Eglibc (at least) need these for C++ support.
     *
     * (Copied from Sourcery CodeBench Lite: arm-none-eabi-gcc -V)
     */
    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > IROM
    
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > IROM
    
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > IROM
    
    . = ALIGN(4);
    
    /* Now we are into the data sections. Those sections are located in various
     * volatile memory locations but have a copy of their contents stored in
     * Flash. Instead of the usual sidata symbol, the GNU LD LOADADDR() is used to.
     */
    .data :
    {
		*(vtable)
		*(.data*)
		. = ALIGN(4);
    } > IRAM AT > IROM
    
    /* The bss section does not contain data, and is instead zeroed. */
    .bss :
    {
		PROVIDE(__bss_start__ = .);
		*(.bss*)
		. = ALIGN(4);
		PROVIDE(__bss_end__ = .);
    } > IRAM
    
    PROVIDE(__stack = ORIGIN(IRAM) + LENGTH(IRAM));
}