#ifndef __SRC_MAIN_H
#define __SRC_MAIN_H

#include "ch32v20x.h"
#include "SystemTick.h"
#include "public.h"
#include "ADC.h"
#include "LED.h"
#include "Key.h"
#include "TM1621.h"
#include "Display.h"
#include "Task.h"
#include "ConfigParam.h"
#include "AT24C02.h"
#include "EEPROM.h"
#include "RTC.h"

//框架元素
#include "Blink.h"
#include "Key_Device.h"
#include "FaultDetection.h"
#include "TimeSilce_polling.h"
#include "ConfigParameters.h"

typedef struct {
    uint16_t Low_Voltage : 1;
    uint16_t High_Voltage : 1;
    uint16_t OverLoad : 1;
    uint16_t Leakage : 1;
    uint16_t High_Temp : 1;
    uint16_t Unalance : 1;
}Error_State_Flag;


typedef struct {
    uint8_t     Cfg_Mode;       //配置模式
    uint8_t     Run_Flag;       //运行中
    uint16_t    Error_State;    //错误状态（那个位 置1 ，对应那个错误）
    uint8_t     Upkeep_Flag;    //保养状态标志
    uint8_t     Motor_Flag;     //电机状态标志
    uint8_t     Pressure_Switch_State;
    uint8_t     Solenoid_valve_Flag;    //电磁阀状态标志
    uint8_t     Preheating_Flag;        //预热状态标志
}Application_State_Flag;

typedef struct {
    uint32_t    Preheating_Time;
    uint32_t    Preheating_Temp_Time;
    uint32_t    Preheating_Detection_Time;
    uint32_t    Upkeep_Detection_Time;
    uint32_t    Upkeep_Detection_Systemopen_Time;
    uint32_t    Temp_Max;
    uint32_t    Temp_Min;
    uint32_t    Voltage_Max;
    uint32_t    Voltage_Min;
    uint32_t    Current;
    uint32_t    Leakage_Current;
}Config_Val_Str;

typedef struct {
    uint32_t Preheating_Temp_Time;
    uint32_t Run_Temp_Time;
    uint32_t System_Open_Time;
    uint32_t RTC_Time;
}Undulation_Time;

#define Task_ID_Prheating       222
#define Task_ID_Prheating_Temp  223
#define Task_ID_Upkeep          221

extern Application_State_Flag App_State;
extern Config_Val_Str ConfigVal;
extern Undulation_Time Undul_Time;

#endif