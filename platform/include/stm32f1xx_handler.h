/*
 * stm32f1xx_handler.h
 *
 *  Created on: Oct 29, 2017
 *      Author: technix
 */

#ifndef HANDLER

#include <stm32f1xx_it.h>

#else

/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
HANDLER(NonMaskableInt, -14)            /*!< 2 Non Maskable Interrupt                             */
HANDLER(HardFault, -13)                 /*!< 3 Cortex-M3 Hard Fault Interrupt                     */
HANDLER(MemoryManagement, -12)          /*!< 4 Cortex-M3 Memory Management Interrupt              */
HANDLER(BusFault, -11)                  /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
HANDLER(UsageFault, -10)                /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
SKIP_HANDLER
HANDLER(SVCall, -5)                     /*!< 11 Cortex-M3 SV Call Interrupt                       */
HANDLER(DebugMonitor, -4)               /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
SKIP_HANDLER
HANDLER(PendSV, -2)                     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
HANDLER(SysTick, -1)                    /*!< 15 Cortex-M3 System Tick Interrupt                   */

/******  STM32 specific Interrupt Numbers *********************************************************/
HANDLER(WWDG, 0)                        /*!< Window WatchDog Interrupt                            */
HANDLER(PVD, 1)                         /*!< PVD through EXTI Line detection Interrupt            */
HANDLER(TAMPER, 2)                      /*!< Tamper Interrupt                                     */
HANDLER(RTC, 3)                         /*!< RTC global Interrupt                                 */
HANDLER(FLASH, 4)                       /*!< FLASH global Interrupt                               */
HANDLER(RCC, 5)                         /*!< RCC global Interrupt                                 */
HANDLER(EXTI0, 6)                       /*!< EXTI Line0 Interrupt                                 */
HANDLER(EXTI1, 7)                       /*!< EXTI Line1 Interrupt                                 */
HANDLER(EXTI2, 8)                       /*!< EXTI Line2 Interrupt                                 */
HANDLER(EXTI3, 9)                       /*!< EXTI Line3 Interrupt                                 */
HANDLER(EXTI4, 10)                      /*!< EXTI Line4 Interrupt                                 */
HANDLER(DMA1_Channel1, 11)              /*!< DMA1 Channel 1 global Interrupt                      */
HANDLER(DMA1_Channel2, 12)              /*!< DMA1 Channel 2 global Interrupt                      */
HANDLER(DMA1_Channel3, 13)              /*!< DMA1 Channel 3 global Interrupt                      */
HANDLER(DMA1_Channel4, 14)              /*!< DMA1 Channel 4 global Interrupt                      */
HANDLER(DMA1_Channel5, 15)              /*!< DMA1 Channel 5 global Interrupt                      */
HANDLER(DMA1_Channel6, 16)              /*!< DMA1 Channel 6 global Interrupt                      */
HANDLER(DMA1_Channel7, 17)              /*!< DMA1 Channel 7 global Interrupt                      */
HANDLER(ADC1_2, 18)                     /*!< ADC1 and ADC2 global Interrupt                       */
HANDLER(USB_HP_CAN1_TX, 19)             /*!< USB Device High Priority or CAN1 TX Interrupts       */
HANDLER(USB_LP_CAN1_RX0, 20)            /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
HANDLER(CAN1_RX1, 21)                   /*!< CAN1 RX1 Interrupt                                   */
HANDLER(CAN1_SCE, 22)                   /*!< CAN1 SCE Interrupt                                   */
HANDLER(EXTI9_5, 23)                    /*!< External Line[9:5] Interrupts                        */
HANDLER(TIM1_BRK, 24)                   /*!< TIM1 Break Interrupt                                 */
HANDLER(TIM1_UP, 25)                    /*!< TIM1 Update Interrupt                                */
HANDLER(TIM1_TRG_COM, 26)               /*!< TIM1 Trigger and Commutation Interrupt               */
HANDLER(TIM1_CC, 27)                    /*!< TIM1 Capture Compare Interrupt                       */
HANDLER(TIM2, 28)                       /*!< TIM2 global Interrupt                                */
HANDLER(TIM3, 29)                       /*!< TIM3 global Interrupt                                */
HANDLER(TIM4, 30)                       /*!< TIM4 global Interrupt                                */
HANDLER(I2C1_EV, 31)                    /*!< I2C1 Event Interrupt                                 */
HANDLER(I2C1_ER, 32)                    /*!< I2C1 Error Interrupt                                 */
HANDLER(I2C2_EV, 33)                    /*!< I2C2 Event Interrupt                                 */
HANDLER(I2C2_ER, 34)                    /*!< I2C2 Error Interrupt                                 */
HANDLER(SPI1, 35)                       /*!< SPI1 global Interrupt                                */
HANDLER(SPI2, 36)                       /*!< SPI2 global Interrupt                                */
HANDLER(USART1, 37)                     /*!< USART1 global Interrupt                              */
HANDLER(USART2, 38)                     /*!< USART2 global Interrupt                              */
HANDLER(USART3, 39)                     /*!< USART3 global Interrupt                              */
HANDLER(EXTI15_10, 40)                  /*!< External Line[15:10] Interrupts                      */
HANDLER(RTC_Alarm, 41)                  /*!< RTC Alarm through EXTI Line Interrupt                */
HANDLER(USBWakeUp, 42)                  /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */

#endif
