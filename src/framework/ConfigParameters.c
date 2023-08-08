#include "ConfigParameters.h"
#include "stdlib.h"
#include "string.h"

static ConfigParameters_Info *CfgParame_Head = NULL;
static ConfigParameters_Info *CfgParame_Now  = NULL;

void ConfigParameters_Creat(uint16_t CfgParame_ID, uint32_t *Val, uint32_t Scale, uint32_t Max, uint32_t Min) {

    ConfigParameters_Info *CfgParame_new = malloc(sizeof(ConfigParameters_Info));
    memset(CfgParame_new, 0, sizeof(ConfigParameters_Info));

    CfgParame_new->CfgParam_Cfg.Val = Val;
    CfgParame_new->CfgParam_Cfg.Max = Max;
    CfgParame_new->CfgParam_Cfg.Min = Min;
    CfgParame_new->CfgParam_Cfg.Scale = Scale;
    CfgParame_new->CfgParame_ID = CfgParame_ID;

    if(CfgParame_Head == NULL) {
        CfgParame_Head = CfgParame_new;
        CfgParame_Now = CfgParame_Head;
    } else {
        ConfigParameters_Info *CfgParame_Info = CfgParame_Head;

        while(CfgParame_Info->CfgParam_next != NULL)  CfgParame_Info = CfgParame_Info->CfgParam_next;
        CfgParame_Info->CfgParam_next = CfgParame_new;
    }
}


uint32_t ConfigParameters_OrderSwitch() {
    if(CfgParame_Head == NULL) return ((uint32_t)0);

    if(CfgParame_Now->CfgParam_next != NULL) {
        CfgParame_Now = CfgParame_Now->CfgParam_next;
    } else {
        CfgParame_Now = CfgParame_Head;
    }

    return *CfgParame_Now->CfgParam_Cfg.Val;
}

uint32_t ConfigParameters_PlusVal() {
    if(CfgParame_Head == NULL) return ((uint32_t)0);
    if(CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;
    if((*CfgParame_Now->CfgParam_Cfg.Val + CfgParame_Now->CfgParam_Cfg.Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
        *CfgParame_Now->CfgParam_Cfg.Val = *CfgParame_Now->CfgParam_Cfg.Val + CfgParame_Now->CfgParam_Cfg.Scale;
    } else {
        *CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
    }
    return *CfgParame_Now->CfgParam_Cfg.Val;
}

uint32_t ConfigParameters_ReduceVal() {
    if(CfgParame_Head == NULL) return ((uint32_t)0);
    if(CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    if((*CfgParame_Now->CfgParam_Cfg.Val - CfgParame_Now->CfgParam_Cfg.Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
        *CfgParame_Now->CfgParam_Cfg.Val = *CfgParame_Now->CfgParam_Cfg.Val - CfgParame_Now->CfgParam_Cfg.Scale;
    } else {
        *CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Min;
    }

    return *CfgParame_Now->CfgParam_Cfg.Val;
}

uint32_t ConfigParameters_GetVal() {
    if(CfgParame_Head == NULL) return ((uint32_t)0);
    if(CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    return *CfgParame_Now->CfgParam_Cfg.Val;
}

uint16_t ConfigParameters_GetID() {
    if(CfgParame_Head == NULL) return (-1);
    if(CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    return CfgParame_Now->CfgParame_ID;
}

void ConfigParameters_GoHeadPage() {
    if(CfgParame_Head == NULL) return ;
    if(CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    CfgParame_Now = CfgParame_Head;
}