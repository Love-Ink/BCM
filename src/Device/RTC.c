
//void RTC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#include "debug.h"
#include "RTC.h"

/* Global define */

/*********************************************************************
 * @fn      RTC_Init
 *
 * @brief   Initializes RTC collection.
 *
 * @return  1 - Init Fail
 *          0 - Init Success
 */
u8 RTC_Init(void)
{
    u8 temp = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    /* Is it the first configuration */
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA1A1)
    {
        BKP_DeInit();
        RCC_LSEConfig(RCC_LSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
        {
            temp++;
            Delay_Ms(20);
        }
        if(temp >= 250)
            return 1;
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        //RTC_ITConfig(RTC_IT_ALR, ENABLE);
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
        RTC_EnterConfigMode();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();
        RTC_SetCounter(0);
        RTC_WaitForLastTask();
        RTC_ExitConfigMode();
        BKP_WriteBackupRegister(BKP_DR1, 0XA1A1);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        PWR_WakeUpPinCmd(DISABLE);
        RTC_WaitForSynchro();
        //RTC_ITConfig(RTC_IT_ALR, ENABLE);
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    return 0;
}







