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
 *    CCM:  Core coupled RAM, used for time-sensitive code and stack.
 */

/* On this runtime environment, the function Reset_Handler is our entry point,
 * instead of the usual _start. This function prepares the clocks, and copies
 * data and CCM code sections in place, before calling _start like usual.
 */
ENTRY(Reset_Handler)

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
	} > CCM AT > IROM = 0xff
	
	/* Now comes the bulk of the application code in Flash. */
	.text :
	{
		*(.text*)
	} > IROM = 0xff
	
	.devices :
	{
		KEEP(*(.devices*))
	} > IROM = 0xff
	
	.rodata :
	{
		*(.rodata*)
		
		/* Address tables */
		PROVIDE(__vector_load = .); LONG(LOADADDR(.vector));
		PROVIDE(__vector_addr = .); LONG(ADDR(.vector));
		PROVIDE(__vector_size = .); LONG(SIZEOF(.vector));
		PROVIDE(__devices_addr = .); LONG(ADDR(.devices));
		PROVIDE(__devices_size = .); LONG(SIZEOF(.devices));
		PROVIDE(__ccm_load = .); LONG(LOADADDR(.ccm));
		PROVIDE(__ccm_addr = .); LONG(ADDR(.ccm));
		PROVIDE(__ccm_size = .); LONG(SIZEOF(.ccm));
		PROVIDE(__data_load = .); LONG(LOADADDR(.data));
		PROVIDE(__data_addr = .); LONG(ADDR(.data));
		PROVIDE(__data_size = .); LONG(SIZEOF(.data));
		PROVIDE(__bss_addr = .); LONG(ADDR(.bss));
		PROVIDE(__bss_size = .); LONG(SIZEOF(.bss));
		PROVIDE(__bss_end = .); LONG(ADDR(.bss) + SIZEOF(.bss));
		PROVIDE(__mem_end = .); LONG(ORIGIN(IRAM) + LENGTH(IRAM));
	} > IROM = 0xff
	
	.init :
	{
		KEEP(*(.init))
	} > IROM = 0xff
	
	.fini :
	{
		KEEP(*(.fini))
	} > IROM = 0xff
	
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
    } > IROM = 0xff
    
    .ARM.exidx : /* index entries for section unwinding */
    {
        *(.ARM.exidx*)
    } > IROM = 0xff

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
    } > IROM = 0xff
    
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > IROM = 0xff
    
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > IROM = 0xff
    
    . = ALIGN(4);
    
    /* Now we are into the data sections. Those sections are located in various
     * volatile memory locations but have a copy of their contents stored in
     * Flash. Instead of the usual sidata symbol, the GNU LD LOADADDR() is used to.
     */
     
    /* There are some code we want to run as fast as possible. Those are placed
     * in the .ccm section here.
     */
    .ccm :
    {
		*(.ccm*)
		. = ALIGN(4);
    } > CCM AT > IROM = 0xff
    
    /* Stack */
    PROVIDE(__stack = ORIGIN(CCM) + LENGTH(CCM));
    
    .data :
    {
		*(vtable)
		*(.data*)
		. = ALIGN(4);
    } > IRAM AT > IROM = 0xff
    
    /* The bss section does not contain data, and is instead zeroed. */
    .bss :
    {
		PROVIDE(__bss_start__ = .);
		*(.bss*)
		. = ALIGN(4);
		PROVIDE(__bss_end__ = .);
    } > IRAM
}