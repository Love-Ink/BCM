/*
 * ch32v203 eide demo
 * version: v1.0
 * Copyright (c) 2022 Taoyukai
 * SPDX-License-Identifier: Apache-2.0
 */

#include "debug.h"
#include "main.h"

Application_State_Flag App_State = { 0, 0, 0, 0, 0, 0, 0, 0};
Config_Val_Str ConfigVal = { 0 };
Undulation_Time Undul_Time = { 0 };


void LED1_trun(uint16_t ms) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, !GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11));
}
void LED2_trun(uint16_t ms) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_10, !GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_10));
}
void LED3_trun(uint16_t ms) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, !GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15));
}

void Every_Init() {
    RTC_Init();
    
    Application_Key_Init();
    Application_LED_Init();
    Application_TM1612_Init();
    Application_CfgParam_Init();
    Applicaiton_EEPROM_Init();

    Application_ADC_Init();

    Application_SysTick_Init(10);
    Project_Display_Clear(0);
}

int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("ch32v203 hello world\r\n");
    
    Every_Init();
    
    App_State.Cfg_Mode = 0;
    App_State.Run_Flag = 0;
    // BlinkDevice_Config("LED1", 100, 100, 0);
    // BlinkDevice_Config("BL", 100, 0, 0);
    BlinkDevice_Config("RunPage", 200, 200, 0);

    CreatTask_Func(100, 10, Key_Scan);
    CreatTask_Func(5 ,400, BlinkDevice_Scan);
    CreatTask_Func(110, 140, Display_Task_Func);
    CreatTask_Func(10, 500,Get_ADC_Measuring);
    CreatTask_Func(12, 400, Device_Control_Task_Func);
    CreatTask_Func(13, 100, Run_State_Control_Task_Func);
    // CreatTask_Func(7, 100, BlinkDevice_Show);
    // CreatTask_Func(9, 2000, ADC_Measuring_show);

    while(1)
    {
        TaskProcess();
    }
}
