#include "Fault.h"


void Application_Fault_Init() {
    FalutDetection_Init FaultDet_InitStructure;

    FaultDet_InitStructure.AutoRemove_Fault = ENABLE;
    FaultDet_InitStructure.Func = NULL;
    FaultDet_InitStructure.Hold_time_set = 5000;
    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Temperature;
    FaultDet_InitStructure.pThreshold = &ConfigVal.Temp_Max;
    FaultDet_InitStructure.Relation = Greater_than_or_Equal_to;
    Creat_FaultDetection("Temp", &FaultDet_InitStructure, ENABLE);  //创建温度监测项目

    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Voltage;
    FaultDet_InitStructure.pThreshold = &ConfigVal.Voltage_Max;
    FaultDet_InitStructure.Relation = Greater_than_or_Equal_to;
    Creat_FaultDetection("HighVol", &FaultDet_InitStructure, ENABLE);  //创建超压监测项目

    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Voltage;
    FaultDet_InitStructure.pThreshold = &ConfigVal.Voltage_Min;
    FaultDet_InitStructure.Relation = Less_than_or_Equal_to;
    Creat_FaultDetection("LowVol", &FaultDet_InitStructure, ENABLE);  //创建欠压监测项目

    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Average_Current;
    FaultDet_InitStructure.pThreshold = &ConfigVal.Current;
    FaultDet_InitStructure.Relation = Greater_than_or_Equal_to;
    Creat_FaultDetection("Overload", &FaultDet_InitStructure, ENABLE);  //创建过载监测项目

    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Leak_Current;
    FaultDet_InitStructure.pThreshold = &ConfigVal.Leakage_Current;
    FaultDet_InitStructure.Relation = Less_than_or_Equal_to;
    Creat_FaultDetection("LeakCur", &FaultDet_InitStructure, ENABLE);  //创建漏电监测项目

    FaultDet_InitStructure.pDet_val = (uint32_t *)&ADC_Measuring.Unbalance_value;
    FaultDet_InitStructure.Threshold = 50;
    FaultDet_InitStructure.Relation = Greater_than;
    Creat_FaultDetection("Unbalance", &FaultDet_InitStructure, ENABLE);  //创建不平衡监测项目


}