/*!
    \file    gd32f4xx_it.c
    \brief   interrupt service routines

    \version 2024-01-15, V3.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx_it.h"
#include "systick.h"
#include "logger.h"

extern void hal_usart0_handle_irq(void);
extern void hal_exti0_handle_irq(void);
extern void hal_exti2_handle_irq(void);
extern void hal_exti5_9_handle_irq(void);
extern void hal_tim5_dac_irqhandler(void);
extern void hal_tim6_dac_irqhandler(void);
extern void hal_dma1_ch0_irqhandler(void);
extern void hal_dma1_ch7_irqhandler(void);

void RTC_Alarm_IRQHandler(void) {
    if (rtc_flag_get(RTC_FLAG_ALRM0) == SET &&
            exti_interrupt_flag_get(EXTI_17) == SET) {
        rtc_flag_clear(RTC_FLAG_ALRM0);
        exti_interrupt_flag_clear(EXTI_17);
        LOG_DEBUG("RTC_Alarm_IRQHandler invoke")
    }
}

void DMA2_Stream0_IRQHandler(void) {
    hal_dma1_ch0_irqhandler();
}

void DMA2_Stream7_IRQHandler(void) {
    hal_dma1_ch7_irqhandler();
}


void USART1_IRQHandler(void) {
    // startup_stm32f407vetx.s 是用的CubeMX生成的，STM32的USART1对应GD32的UART0
    hal_usart0_handle_irq();
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void) {
    /* if NMI exception occurs, go to infinite loop */
    LOG_ERROR("NMI exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void) {
    /* if Hard Fault exception occurs, go to infinite loop */
    LOG_ERROR("Hard Fault exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void) {
    /* if Memory Manage exception occurs, go to infinite loop */
    LOG_ERROR("Memory Manage exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void) {
    /* if Bus Fault exception occurs, go to infinite loop */
    LOG_ERROR("Bus Fault exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void) {
    /* if Usage Fault exception occurs, go to infinite loop */
    LOG_ERROR("Usage Fault exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void) {
    /* if SVC exception occurs, go to infinite loop */
    LOG_ERROR("SVC exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void) {
    /* if DebugMon exception occurs, go to infinite loop */
    LOG_ERROR("DebugMon exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void) {
    /* if PendSV exception occurs, go to infinite loop */
    LOG_ERROR("PendSV exception occurs")
    while (1) {
    }
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void) {
    delay_decrement();
}
void EXTI0_IRQHandler(void) {
    hal_exti0_handle_irq();
}

void EXTI2_IRQHandler(void) {
    hal_exti2_handle_irq();
}

void EXTI9_5_IRQHandler(void) {
    hal_exti5_9_handle_irq();
}

/**
 * @note STM32中的TIM6_DAC_IRQHandler对应GD32中的TIM6_DAC_IRQHandler
 */
void TIM6_DAC_IRQHandler() {
    hal_tim5_dac_irqhandler();
}

void TIM7_IRQHandler() {
    hal_tim6_dac_irqhandler();
}
