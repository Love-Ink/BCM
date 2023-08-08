#ifndef __APPLICATION_SYSTEMTICK_H
#define __APPLICATION_SYSTEMTICK_H

#include "ch32v20x.h"
#include "main.h"

typedef struct {
    uint32_t STE    : 1;    //系统计数器使能控制位
    uint32_t STIE   : 1;    //计数器中断使能控制位
    uint32_t STCLK  : 1;    //计数器时钟源选择位(是否8分频)
    uint32_t STRE   : 1;    //自动重装载计数使能位
    uint32_t MODE   : 1;    //计数模式
    uint32_t INIT   : 1;    //计数器初始值更新
    uint32_t        : 25;   //保留
    uint32_t SWIE   : 1;    //软件中断触发使能
}Application_Systick_CTLR;

#define SysTick_CTLR    ((Application_Systick_CTLR *)(&SysTick->CTLR))
extern uint16_t systick_ms;

void Application_SysTick_Init(uint16_t ms);
void Application_SysTick_SR_Clear();

extern void SysTick_Handler(void)  __attribute__((interrupt("WCH-Interrupt-fast")));

#endif