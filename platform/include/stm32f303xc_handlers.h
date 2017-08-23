/*
 * stm32f303xc_handlers.h
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

// This file does not have an include guard, as it is intended to be included
// in multiple places, for multiple times.

/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
HANDLER(NonMaskableInt, -14)            /*!< 2 Non Maskable Interrupt                                          */
HANDLER(HardFault, -13)                 /*!< 3 Hard Fault                                                      */
HANDLER(MemoryManagement, -12)          /*!< 4 Cortex-M4 Memory Management Interrupt                           */
HANDLER(BusFault, -11)                  /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
HANDLER(UsageFault, -10)                /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
HANDLER(SVCall, -5)                     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
HANDLER(DebugMonitor, -4)               /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
SKIP_HANDLER
HANDLER(PendSV, -2)                     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
HANDLER(SysTick, -1)                    /*!< 15 Cortex-M4 System Tick Interrupt                                */

/******  STM32 specific Interrupt Numbers **********************************************************************/
HANDLER(WWDG, 0)                        /*!< Window WatchDog Interrupt                                         */
HANDLER(PVD, 1)                         /*!< PVD through EXTI Line detection Interrupt                         */
HANDLER(TAMP_STAMP, 2)                  /*!< Tamper and TimeStamp interrupts through the EXTI line 19          */
HANDLER(RTC_WKUP, 3)                    /*!< RTC Wakeup interrupt through the EXTI line 20                     */
HANDLER(FLASH, 4)                       /*!< FLASH global Interrupt                                            */
HANDLER(RCC, 5)                         /*!< RCC global Interrupt                                              */
HANDLER(EXTI0, 6)                       /*!< EXTI Line0 Interrupt                                              */
HANDLER(EXTI1, 7)                       /*!< EXTI Line1 Interrupt                                              */
HANDLER(EXTI2_TSC, 8)                   /*!< EXTI Line2 Interrupt and Touch Sense Controller Interrupt         */
HANDLER(EXTI3, 9)                       /*!< EXTI Line3 Interrupt                                              */
HANDLER(EXTI4, 10)                      /*!< EXTI Line4 Interrupt                                              */
HANDLER(DMA1_Channel1, 11)              /*!< DMA1 Channel 1 Interrupt                                          */
HANDLER(DMA1_Channel2, 12)              /*!< DMA1 Channel 2 Interrupt                                          */
HANDLER(DMA1_Channel3, 13)              /*!< DMA1 Channel 3 Interrupt                                          */
HANDLER(DMA1_Channel4, 14)              /*!< DMA1 Channel 4 Interrupt                                          */
HANDLER(DMA1_Channel5, 15)              /*!< DMA1 Channel 5 Interrupt                                          */
HANDLER(DMA1_Channel6, 16)              /*!< DMA1 Channel 6 Interrupt                                          */
HANDLER(DMA1_Channel7, 17)              /*!< DMA1 Channel 7 Interrupt                                          */
HANDLER(ADC1_2, 18)                     /*!< ADC1 & ADC2 Interrupts                                            */
HANDLER(USB_HP_CAN_TX, 19)              /*!< USB Device High Priority or CAN TX Interrupts                     */
HANDLER(USB_LP_CAN_RX0, 20)             /*!< USB Device Low Priority or CAN RX0 Interrupts                     */
HANDLER(CAN_RX1, 21)                    /*!< CAN RX1 Interrupt                                                 */
HANDLER(CAN_SCE, 22)                    /*!< CAN SCE Interrupt                                                 */
HANDLER(EXTI9_5, 23)                    /*!< External Line[9:5] Interrupts                                     */
HANDLER(TIM1_BRK_TIM15, 24)             /*!< TIM1 Break and TIM15 Interrupts                                   */
HANDLER(TIM1_UP_TIM16, 25)              /*!< TIM1 Update and TIM16 Interrupts                                  */
HANDLER(TIM1_TRG_COM_TIM17, 26)         /*!< TIM1 Trigger and Commutation and TIM17 Interrupt                  */
HANDLER(TIM1_CC, 27)                    /*!< TIM1 Capture Compare Interrupt                                    */
HANDLER(TIM2, 28)                       /*!< TIM2 global Interrupt                                             */
HANDLER(TIM3, 29)                       /*!< TIM3 global Interrupt                                             */
HANDLER(TIM4, 30)                       /*!< TIM4 global Interrupt                                             */
HANDLER(I2C1_EV, 31)                    /*!< I2C1 Event Interrupt & EXTI Line23 Interrupt (I2C1 wakeup)        */
HANDLER(I2C1_ER, 32)                    /*!< I2C1 Error Interrupt                                              */
HANDLER(I2C2_EV, 33)                    /*!< I2C2 Event Interrupt & EXTI Line24 Interrupt (I2C2 wakeup)        */
HANDLER(I2C2_ER, 34)                    /*!< I2C2 Error Interrupt                                              */
HANDLER(SPI1, 35)                       /*!< SPI1 global Interrupt                                             */
HANDLER(SPI2, 36)                       /*!< SPI2 global Interrupt                                             */
HANDLER(USART1, 37)                     /*!< USART1 global Interrupt & EXTI Line25 Interrupt (USART1 wakeup)   */
HANDLER(USART2, 38)                     /*!< USART2 global Interrupt & EXTI Line26 Interrupt (USART2 wakeup)   */
HANDLER(USART3, 39)                     /*!< USART3 global Interrupt & EXTI Line28 Interrupt (USART3 wakeup)   */
HANDLER(EXTI15_10, 40)                  /*!< External Line[15:10] Interrupts                                   */
HANDLER(RTC_Alarm, 41)                  /*!< RTC Alarm (A and B) through EXTI Line 17 Interrupt                */
HANDLER(USBWakeUp, 42)                  /*!< USB Wakeup Interrupt                                              */
HANDLER(TIM8_BRK, 43)                   /*!< TIM8 Break Interrupt                                              */
HANDLER(TIM8_UP, 44)                    /*!< TIM8 Update Interrupt                                             */
HANDLER(TIM8_TRG_COM, 45)               /*!< TIM8 Trigger and Commutation Interrupt                            */
HANDLER(TIM8_CC, 46)                    /*!< TIM8 Capture Compare Interrupt                                    */
HANDLER(ADC3, 47)                       /*!< ADC3 global Interrupt                                             */
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
HANDLER(SPI3, 51)                       /*!< SPI3 global Interrupt                                             */
HANDLER(UART4, 52)                      /*!< UART4 global Interrupt & EXTI Line34 Interrupt (UART4 wakeup)     */
HANDLER(UART5, 53)                      /*!< UART5 global Interrupt & EXTI Line35 Interrupt (UART5 wakeup)     */
HANDLER(TIM6_DAC, 54)                   /*!< TIM6 global and DAC channel 1&2 underrun error  interrupts        */
HANDLER(TIM7, 55)                       /*!< TIM7 global Interrupt                                             */
HANDLER(DMA2_Channel1, 56)              /*!< DMA2 Channel 1 global Interrupt                                   */
HANDLER(DMA2_Channel2, 57)              /*!< DMA2 Channel 2 global Interrupt                                   */
HANDLER(DMA2_Channel3, 58)              /*!< DMA2 Channel 3 global Interrupt                                   */
HANDLER(DMA2_Channel4, 59)              /*!< DMA2 Channel 4 global Interrupt                                   */
HANDLER(DMA2_Channel5, 60)              /*!< DMA2 Channel 5 global Interrupt                                   */
HANDLER(ADC4, 61)                       /*!< ADC4  global Interrupt                                            */
SKIP_HANDLER
SKIP_HANDLER
HANDLER(COMP1_2_3, 64)                  /*!< COMP1, COMP2 and COMP3 global Interrupt via EXTI Line21, 22 and 29*/
HANDLER(COMP4_5_6, 65)                  /*!< COMP4, COMP5 and COMP6 global Interrupt via EXTI Line30, 31 and 32*/
HANDLER(COMP7, 66)                      /*!< COMP7 global Interrupt via EXTI Line33                            */
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
HANDLER(USB_HP, 74)                     /*!< USB High Priority global Interrupt remap                          */
HANDLER(USB_LP, 75)                     /*!< USB Low Priority global Interrupt  remap                          */
HANDLER(USBWakeUp_RMP, 76)              /*!< USB Wakeup Interrupt remap                                        */
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
HANDLER(FPU, 81)                        /*!< Floating point Interrupt                                          */
