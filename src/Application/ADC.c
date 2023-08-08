#include "ADC.h"
#include "public.h"
#include "string.h"
static int16_t Calibrattion_Val = 0;

struct ADC_Measuring_Info ADC_Measuring = { 0 };
void Application_ADC_Init() {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
    gpio_init(GPIOA, GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AIN);
    gpio_init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_AIN);

    ADC_InitTypeDef ADC_InitStructure = {0};

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);
}

/**
 * @brief DMA初始化
 * 
 * @param DMAy_Channelx DMA通道 
 * @param Data_from     数据从哪里取
 * @param Data_to       数据放到哪里
 * @param memory_size   数据存放区大小
 */
void Application_DMA_Init(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t Data_from, uint32_t Data_to, uint16_t memory_size) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure = {0};
    
    DMA_DeInit(DMAy_Channelx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = Data_from;
    DMA_InitStructure.DMA_MemoryBaseAddr = Data_to;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = memory_size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMAy_Channelx, &DMA_InitStructure);

}

uint16_t Get_ConversionVal(int16_t val)
{
    if((val + Calibrattion_Val) < 0)
        return 0;
    if((Calibrattion_Val + val) > 4095||val==4095)
        return 4095;
    return (val + Calibrattion_Val);
}

u16 Get_ADC_Val(u8 ch)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    val = ADC_GetConversionValue(ADC1);

    return val;
}

uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)        //求多次采集数值的平均值
{
    uint64_t temp_val=0,get_val;
    uint32_t temp_big=0,temp_small=4095;
    uint8_t t;
    if(times <=2)times = 4;
    for(t=0;t<times;t++)
    {
        get_val = Get_ConversionVal(Get_ADC_Val(ch));
        temp_val += get_val;
        if(get_val > temp_big) temp_big = get_val;
        if(get_val < temp_small) temp_small = get_val;
    }
    temp_val = (temp_val - temp_big - temp_small);
    return temp_val/(times-2);
}

int16_t GET_Temperature(int16_t u0)//获取PT100采集温度值函数
{
    int16_t Temperture,PT100_Val,PT100_Res;
    PT100_Val = ((u0)+11.875*300)/11.875;//PT100电压
    PT100_Res = (1000)*(PT100_Val)/(3300-(PT100_Val));//PT100电阻
    Temperture = (258.74)*(PT100_Res)-(25845);

    return Temperture;
}

void Get_ADC_Measuring(uint16_t ms){
    uint32_t ADC_Val = 0;
    ADC_Val = Get_Adc_Average(ADC_Channel_5, 10);
    ADC_Measuring.Voltage = (float)((-12.81) * (ADC_Val) + (49844.0)) / 100.0;//电压校准

    ADC_Val = Get_Adc_Average(ADC_Channel_6, 10);
    ADC_Measuring.Current1 = ((1.67) * (ADC_Val) + (23.08)) / 100.0;//电流1校准

    ADC_Val = Get_Adc_Average(ADC_Channel_9, 10);
    ADC_Measuring.Current2 = ((1.67) * (ADC_Val) + (25.76)) / 100.0;//电流2校准

    ADC_Val = Get_Adc_Average(ADC_Channel_7, 10);
    ADC_Measuring.Current3 = ((1.67) * (ADC_Val) + (25.17)) / 100.0;//电流3校准

    ADC_Val = Get_Adc_Average(ADC_Channel_8, 10);
    ADC_Measuring.Leak_Current = ((8.03) * (ADC_Val) + (259.39)) / 100.0;//漏电流校准

    ADC_Measuring.Average_Current = (ADC_Measuring.Current1 + ADC_Measuring.Current2 + ADC_Measuring.Current3)/3.0;//电流值
    if(ADC_Measuring.Average_Current < 100 ) ADC_Measuring.Average_Current = 0;

    ADC_Val = Get_Adc_Average(ADC_Channel_1, 20);;//ADC-T输出电压采集值
    ADC_Measuring.Temperature = GET_Temperature(ADC_Val) / 100;//实际温度

}

void ADC_Measuring_show(uint16_t ms) {

    printf("\r\n");
    printf("Voltage : %f\r\n", ADC_Measuring.Voltage);
    printf("Leak_Current : %f\r\n", ADC_Measuring.Leak_Current);
    printf("Average_Current : %f\r\n", ADC_Measuring.Average_Current);
    printf("Temperature : %f\r\n", ADC_Measuring.Temperature);
    printf("test : %f\r\n", 123.456);
}