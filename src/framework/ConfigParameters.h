#ifndef __FRAMEWORK_CONFIG_PARAMETERS_H
#define __FRAMEWORK_CONFIG_PARAMETERS_H

#include "ch32v20x.h"

typedef struct ConfigParameters_Config{
    uint32_t *Val;
    uint32_t Max;
    uint32_t Min;
    uint32_t Scale;
}ConfigParameters_Init;

typedef struct ConfigParameters_Info{
    uint16_t CfgParame_ID;

    ConfigParameters_Init CfgParam_Cfg;
    struct ConfigParameters_Info *CfgParam_next;
}ConfigParameters_Info;

void ConfigParameters_Creat(uint16_t CfgParame_ID, uint32_t *Val, uint32_t Scale, uint32_t Max, uint32_t Min);
uint32_t ConfigParameters_OrderSwitch();
uint32_t ConfigParameters_PlusVal();
uint32_t ConfigParameters_ReduceVal();
uint32_t ConfigParameters_GetVal();
uint16_t ConfigParameters_GetID();
void ConfigParameters_GoHeadPage();
#endif