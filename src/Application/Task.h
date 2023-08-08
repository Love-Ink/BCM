#ifndef __APPLICATION_TASK_H
#define __APPLICATION_TASK_H

#include "ch32v20x.h"
#include "main.h"

typedef enum {
    CFG_MODE_Preheating_Temp_Time_PAGE = 1,
    CFG_MODE_Preheating_Time_PAGE ,
    CFG_MODE_Preheating_Detection_Time_PAGE,
    CFG_MODE_Upkeep_Detection_Time_PAGE,
    CFG_MODE_Upkeep_Detection_Systemopen_Time_PAGE,
    CFG_MODE_Temp_Max_PAGE,
    CFG_MODE_Temp_Min_PAGE,
    CFG_MODE_Voltage_Max_PAGE,
    CFG_MODE_Voltage_Min_PAGE,
    CFG_MODE_Current_Max_PAGE,
    CFG_MODE_Leakage_Current_Max_PAGE
}CfgMode_PageIndex;

typedef enum {
    RUN_PAGE_VOLTAGE = 0,
    RUN_PAGE_CURRENT,
    RUN_PAGE_LEAKAGE,
    RUN_PAGE_TEMP,
    RUN_PAGE_NUMS
}RunPage_Index;

typedef enum {
    ERROR_PAGE_LOWVOLTAGE = 0,
    ERROR_PAGE_HIGHVOLTAGE,
    ERROR_PAGE_OVERLOAD,
    ERROR_PAGE_LEAKAGE,
    ERROR_PAGE_UNBALANCE,
    ERROR_PAGE_TEMPERATURE,
    ERROR_PAGE_NUMS
}ErrorPage_Index;

extern uint16_t Error_Page, Run_Page;

void Display_Task_Func(uint16_t ms);
void Device_Control_Task_Func(uint16_t ms);
void Run_State_Control_Task_Func (uint16_t ms);
#endif