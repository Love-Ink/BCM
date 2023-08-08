#include "SystemTick.h"

uint16_t systick_ms = 0;

/**
 * @brief 系统滴答定时器初始化
 * 
 */
void Application_SysTick_Init(uint16_t ms) {
    
    SysTick_CTLR->SWIE = 0;
    SysTick_CTLR->INIT = 1;
    SysTick_CTLR->MODE = 1;
    SysTick_CTLR->STRE = 1;
    SysTick_CTLR->STCLK = 0;
    SysTick_CTLR->STIE = 1;
    SysTick_CTLR->STE = 1;

    SysTick->CMP = ((SystemCoreClock / 8) / 1000) * ms;
    NVIC_InitTypeDef NVIC_InitStructure;
    systick_ms = ms;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = SysTicK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief 清除系统滴答定时器中断标志位
 * 
 */
void Application_SysTick_SR_Clear() {

    SysTick->SR = 0x00;

}

/**
 * @brief 系统滴答定时器中断函数
 * 
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void) {
    if((SysTick->SR & 0x01) == SET) {

        TaskRemarks();
        // GPIO_WriteBit(GPIOA, GPIO_Pin_11, !GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11));
        Application_SysTick_SR_Clear();
    }
}