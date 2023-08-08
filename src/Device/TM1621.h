/*
 * TM1621.h
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

#ifndef APPLICATION_TM1621_H_
#define APPLICATION_TM1621_H_

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//--------------------------------------------------------------------------------------------------
// @文件包含引用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "ch32v20x.h"
//--------------------------------------------------------------------------------------------------
// @宏定义宏声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 代码宏定义
#define TM1621_SUCCESS                      (0)             // TM1621 成功
#define TM1621_FAILURE                      (1)             // TM1621 失败
#define TM1621_UNKNOWN                      (2)             // TM1621 未知

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 时钟源选项宏定义
#define TM1621_CLOCK_XTAL32K                (0)             // TM1621 系统时钟源选择 外部晶振32.768KHZ
#define TM1621_CLOCK_RC256K                 (1)             // TM1621 系统时钟源选择 片内RC振荡器256KHz
#define TM1621_CLOCK_EXT256K                (2)             // TM1621 系统时钟源选择 外部时钟源256KHz

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 偏压电压选项宏定义
#define TM1621_BIAS_1P2                     (0)             // TM1621 偏压电压选择 1/2偏置电压
#define TM1621_BIAS_1P3                     (1)             // TM1621 偏压电压选择 1/3偏置电压

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TM1621 公共口选项宏定义
#define TM1621_COM_2                        (0)             // TM1621 公共口选择 使用2个公共口
#define TM1621_COM_3                        (1)             // TM1621 公共口选择 使用3个公共口
#define TM1621_COM_4                        (2)             // TM1621 公共口选择 使用4个公共口

//--------------------------------------------------------------------------------------------------
// @变量定义声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
typedef struct
{
    uint8_t Clock:2;                                // TM1621 时钟选择  取值范围 - 详情见TM1621_CLOCK宏定义说明
    uint8_t Bias:2;                                 // TM1621 偏压选项  取值范围 - 详情见TM1621_BIAS宏定义说明
    uint8_t Common:2;                               // TM1621 公共口选项  取值范围 - 详情见TM1621_COM宏定义说明

    void (*CS_GPIO_ResetBit_Function)(void);        // TM1621 CS/片选引脚 GPIO清除低电平函数
    void (*CS_GPIO_SetBit_Function)(void);          // TM1621 CS/片选引脚 GPIO置位高电平函数
    void (*WR_GPIO_ResetBit_Function)(void);        // TM1621 WR/写引脚 GPIO清除低电平函数
    void (*WR_GPIO_SetBit_Function)(void);          // TM1621 WR/写引脚 GPIO置位高电平函数
    void (*RD_GPIO_ResetBit_Function)(void);        // TM1621 RD/读引脚 GPIO清除低电平函数
    void (*RD_GPIO_SetBit_Function)(void);          // TM1621 RD/读引脚 GPIO置位高电平函数
    void (*DATA_GPIO_ResetBit_Function)(void);      // TM1621 DATA/数据引脚 GPIO清除低电平函数
    void (*DATA_GPIO_SetBit_Function)(void);        // TM1621 DATA/数据引脚 GPIO置位高电平函数
    void (*DELAY_nUS_Function)(uint16_t Time);      // TM1621 DELAY延迟微秒函数
} TM1621_Handle_t;

extern TM1621_Handle_t     Project_TM1621;
//--------------------------------------------------------------------------------------------------
// @函数声明调用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// TM1621 初始化
void TM1621_StructInit(TM1621_Handle_t *TM1621_Handle);
uint8_t TM1621_Init(TM1621_Handle_t *TM1621_Handle);

// TM1621 写数据
void TM1621_WriteByte(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t Byte);
void TM1621_WriteBuffer(TM1621_Handle_t *TM1621_Handle, uint8_t Address, uint8_t *Buffer, uint8_t Count);

// TM1621 显示/清除 屏幕
void TM1621_ShowAll(TM1621_Handle_t *TM1621_Handle);
void TM1621_ClearAll(TM1621_Handle_t *TM1621_Handle);

void Application_TM1612_Init();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif /* APPLICATION_TM1621_H_ */
