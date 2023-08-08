#ifndef __APPLICATION_ADC_H
#define __APPLICATION_ADC_H

#include "ch32v20x.h"
#include "main.h"


struct ADC_Measuring_Info{
    float Voltage;            //电压
    float Current1;           //第一路电流
    float Current2;           //第二路电流
    float Current3;           //第三路电流
    float Average_Current;    //平均电流
    float Leak_Current;       //漏电
    float Unbalance_value;    //不平衡
    float Temperature;        //温度
};

extern struct ADC_Measuring_Info ADC_Measuring; 

void Application_ADC_Init();
void Application_DMA_Init(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t Data_to, uint32_t Data_from, uint16_t memory_size);
uint16_t Get_ConversionVal(int16_t val);
void Get_ADC_Measuring(uint16_t ms);
void ADC_Measuring_show(uint16_t ms);
#endif