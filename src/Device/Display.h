/*
 * Display.h
 *
 *  Created on: Mar 28, 2023
 *      Author: bbhn2
 */

#ifndef APPLICATION_DISPLAY_H_
#define APPLICATION_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//--------------------------------------------------------------------------------------------------
//  头部文件包含    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "TM1621.h"

//--------------------------------------------------------------------------------------------------
// @宏定义宏声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
//  工程 显示状态 索引
#define PROJECT_DISPLAYSTATE_INDEX_RUN              (0)             // 运行
#define PROJECT_DISPLAYSTATE_INDEX_WAIT             (1)             // 待机
#define PROJECT_DISPLAYSTATE_INDEX_STOP             (2)             // 停机

//  工程 显示参数 索引
// #define PROJECT_DISPLAYPARAM_INDEX_VOLTAGE          (PROJECT_DISPLAYDATA_VOLTAGE_SHIFT)     // 电压
// #define PROJECT_DISPLAYPARAM_INDEX_CURRENT          (PROJECT_DISPLAYDATA_CURRENT_SHIFT)     // 电流
// #define PROJECT_DISPLAYPARAM_INDEX_LEAKAGE          (PROJECT_DISPLAYDATA_LEAKAGE_SHIFT)     // 漏电
// #define PROJECT_DISPLAYPARAM_INDEX_POWER            (PROJECT_DISPLAYDATA_POWER_SHIFT)       // 功率
// #define PROJECT_DISPLAYPARAM_INDEX_TEMP             (PROJECT_DISPLAYDATA_TEMP_SHIFT)        // 温度
//#define PROJECT_DISPLAYPARAM_INDEX_VOLTAGE          (1)     // 电压
//#define PROJECT_DISPLAYPARAM_INDEX_CURRENT          (2)     // 电流
//#define PROJECT_DISPLAYPARAM_INDEX_LEAKAGE          (3)     // 漏电
//#define PROJECT_DISPLAYPARAM_INDEX_POWER            (4)     // 功率
// #define PROJECT_DISPLAYPARAM_INDEX_ENERGYNOW        (PROJECT_DISPLAYDATA_ENERGYNOW_SHIFT)   // 当前电能
// #define PROJECT_DISPLAYPARAM_INDEX_ENERGYALL        (PROJECT_DISPLAYDATA_ENERGYALL_SHIFT)   // 累计电能
// #define PROJECT_DISPLAYPARAM_INDEX_TEMPERATURE      (PROJECT_DISPLAYDATA_TEMPERATURE_SHIFT) // 温度
// #define PROJECT_DISPLAYPARAM_INDEX_TIMEREAL         (PROJECT_DISPLAYDATA_TIMEREAL_SHIFT)    // 实时时钟

#define PROJECT_ERRORFAULTCODE_LEAKAGE              (100)           // 错误故障码 设备故障 漏电故障
#define PROJECT_ERRORFAULTCODE_OVERLOAD             (101)           // 错误故障码 设备故障 过载故障
#define PROJECT_ERRORFAULTCODE_NULLLOAD             (102)           // 错误故障码 设备故障 空载故障
#define PROJECT_ERRORFAULTCODE_LOWVOLTAGE           (103)           // 错误故障码 设备故障 欠压故障
#define PROJECT_ERRORFAULTCODE_HIGHVOLTAGE          (104)           // 错误故障码 设备故障 超压故障
#define PROJECT_ERRORFAULTCODE_LACKPHASE            (105)           // 错误故障码 设备故障 缺相故障
#define PROJECT_ERRORFAULTCODE_UNBALANCE            (106)           // 错误故障码 设备故障 不平衡度故障
#define PROJECT_ERRORFAULTCODE_FASTTURN             (107)           // 错误故障码 设备故障 频繁启停故障
#define PROJECT_ERRORFAULTCODE_LEAKWATER            (108)           // 错误故障码 设备故障 漏水故障
#define PROJECT_ERRORFAULTCODE_OILCHANGE            (109)           // 错误故障码 设备故障 换油故障
#define PROJECT_ERRORFAULTCODE_TEMPERATURE          (110)           // 错误故障码 设备故障 温度故障
#define PROJECT_ERRORFAULTCODE_FITTING              (111)           // 错误故障码 设备故障 保养故障
#define PROJECT_ERRORFAULTCODE_ELSEERROR            (112)           // 错误故障码 设备故障 其他故障

//  工程 显示故障 掩码
#define PROJECT_DISPLAYFAULT_MASK_NULL              (PROJECT_FAULT_NULL_MASK)               // 无
#define PROJECT_DISPLAYFAULT_MASK_LEAKAGE           (PROJECT_FAULT_LEAKAGE_MASK)            // 漏电
#define PROJECT_DISPLAYFAULT_MASK_OVERLOAD          (PROJECT_FAULT_OVERLOAD_MASK)           // 过载
#define PROJECT_DISPLAYFAULT_MASK_NULLLOAD          (PROJECT_FAULT_NULLLOAD_MASK)           // 空载
#define PROJECT_DISPLAYFAULT_MASK_LOWVOLTAGE        (PROJECT_FAULT_LOWVOLTAGE_MASK)         // 欠压
#define PROJECT_DISPLAYFAULT_MASK_HIGHVOLTAGE       (PROJECT_FAULT_HIGHVOLTAGE_MASK)        // 超压
#define PROJECT_DISPLAYFAULT_MASK_LACKPHASE         (PROJECT_FAULT_LACKPHASE_MASK)          // 缺相
#define PROJECT_DISPLAYFAULT_MASK_UNBALANCE         (PROJECT_FAULT_UNBALANCE_MASK)          // 不平衡度
#define PROJECT_DISPLAYFAULT_MASK_FASTTURN          (PROJECT_FAULT_FASTTURN_MASK)           // 频繁启停
#define PROJECT_DISPLAYFAULT_MASK_LEAKWATER         (PROJECT_FAULT_LEAKWATER_MASK)          // 漏水
#define PROJECT_DISPLAYFAULT_MASK_OILCHANGE         (PROJECT_FAULT_OILCHANGE_MASK)          // 换油
#define PROJECT_DISPLAYFAULT_MASK_TEMPERATURE       (PROJECT_FAULT_TEMPERATURE_MASK)        // 温度
#define PROJECT_DISPLAYFAULT_MASK_ELSEERROR         (PROJECT_FAULT_ELSEERROR_MASK)          // 其他

//--------------------------------------------------------------------------------------------------
// @变量定义声明    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
//  启用对匿名结构和联合的支持
#if 1
#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning 586
#elif defined ( __CSMC__ )
    /* anonymous unions are enabled by default */
#else
    #warning Not supported compiler type
#endif
#endif
typedef union
{
    struct
    {
        // SEG0接屏幕引脚9，分别为S9、S16、S20、P4，按COM1、COM2、COM3、COM4按顺序
        uint64_t Param_Temp:1;              // 温度 对应S9
        uint64_t Unit_C:1;                  // 单位 ℃ 温度单位 对应S16
        uint64_t Unit_V:1;                  // 单位 V 电压单位 对应S20
        uint64_t SEG_4_DP:1;                // 数码管 第4位DP段 对应P4

        // SEG1接屏幕引脚10，分别为S10、S15、S17、S21，按COM1、COM2、COM3、COM4按顺序
        uint64_t Function_Debug:1;          // 调试功能 对应S10
        uint64_t Unit_1000:1;               // 单位 1000倍 对应S15
        uint64_t Unit_K:1;                  // 单位 K 配合KW/KWh 功率/电能单位 对应S17
        uint64_t Unit_M:1;                  // 单位 M 配合MA 电流单位 对应S21

        // SEG2接屏幕引脚11，分别为S11、S19、S18、S22，按COM1、COM2、COM3、COM4按顺序
        uint64_t Function_Match:1;          // 配对功能 对应S11
        uint64_t Unit_h:1;                  // 单位 h KWh 功率/时间单位 对应S19
        uint64_t Unit_W:1;                  // 单位 W 配合KW/KWh 功率/电能单位 对应S18
        uint64_t Unit_A:1;                  // 单位 A 电流单位 对应S22

        // SEG3接屏幕引脚12，分别为S8、S7、S6、S5，按COM1、COM2、COM3、COM4按顺序
        uint64_t Param_Time:1;              // 时间 对应S8
        uint64_t Param_Energy:1;            // 电能 对应S7
        uint64_t Param_Power:1;             // 功率 对应S6
        uint64_t Param_Current:1;           // 电流 对应S5

        // SEG4接屏幕引脚13，分别为S1、S2、S3、S4，按COM1、COM2、COM3、COM4按顺序
        uint64_t Param_Mode:1;              // 模式 对应S1
        uint64_t Param_Total:1;             // 累计 对应S2
        uint64_t Param_Voltage:1;           // 电压 对应S3
        uint64_t Param_Leakage:1;           // 漏电 对应S4

        // SEG5接屏幕引脚14，分别为4B、4G、4C、COL，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_4_B:1;                 // 数码管 第4位B段 对应4B
        uint64_t SEG_4_G:1;                 // 数码管 第4位G段 对应4G
        uint64_t SEG_4_C:1;                 // 数码管 第4位C段 对应4C
        uint64_t SEG_Time:1;                // 数码管 时间段 对应COL

        // SEG6接屏幕引脚15，分别为4A、4F、4E、4D，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_4_A:1;                 // 数码管 第4位A段 对应4A
        uint64_t SEG_4_F:1;                 // 数码管 第4位F段 对应4F
        uint64_t SEG_4_E:1;                 // 数码管 第4位E段 对应4E
        uint64_t SEG_4_D:1;                 // 数码管 第4位D段 对应4D

        // SEG7接屏幕引脚16，分别为3B、3G、3C、P3，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_3_B:1;                 // 数码管 第3位B段 对应3B
        uint64_t SEG_3_G:1;                 // 数码管 第3位G段 对应3G
        uint64_t SEG_3_C:1;                 // 数码管 第3位C段 对应3C
        uint64_t SEG_3_DP:1;                // 数码管 第3位DP段 对应P3

        // SEG8接屏幕引脚17，分别为3A、3F、3E、3D，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_3_A:1;                 // 数码管 第3位A段 对应3A
        uint64_t SEG_3_F:1;                 // 数码管 第3位F段 对应3F
        uint64_t SEG_3_E:1;                 // 数码管 第3位E段 对应3E
        uint64_t SEG_3_D:1;                 // 数码管 第3位D段 对应3D

        // SEG9接屏幕引脚18，分别为2B、2G、2C、P2，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_2_B:1;                 // 数码管 第2位B段 对应2B
        uint64_t SEG_2_G:1;                 // 数码管 第2位G段 对应2G
        uint64_t SEG_2_C:1;                 // 数码管 第2位C段 对应2C
        uint64_t SEG_2_DP:1;                // 数码管 第2位DP段 对应P2

        // SEG10接屏幕引脚19，分别为2A、2F、2E、2D，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_2_A:1;                 // 数码管 第2位A段 对应2A
        uint64_t SEG_2_F:1;                 // 数码管 第2位F段 对应2F
        uint64_t SEG_2_E:1;                 // 数码管 第2位E段 对应2E
        uint64_t SEG_2_D:1;                 // 数码管 第2位D段 对应2D

        // SEG11接屏幕引脚20，分别为1B、1G、1C、P1，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_1_B:1;                 // 数码管 第1位B段 对应1B
        uint64_t SEG_1_G:1;                 // 数码管 第1位G段 对应1G
        uint64_t SEG_1_C:1;                 // 数码管 第1位C段 对应1C
        uint64_t SEG_1_DP:1;                // 数码管 第1位DP段 对应P1

        // SEG12接屏幕引脚1，分别为1A、1F、1E、1D，按COM1、COM2、COM3、COM4按顺序
        uint64_t SEG_1_A:1;                 // 数码管 第1位A段 对应1A
        uint64_t SEG_1_F:1;                 // 数码管 第1位F段 对应1F
        uint64_t SEG_1_E:1;                 // 数码管 第1位E段 对应1E
        uint64_t SEG_1_D:1;                 // 数码管 第1位D段 对应1D

        // SEG13接屏幕引脚2，分别为S12、S13、S14、S23，按COM1、COM2、COM3、COM4按顺序
        uint64_t State_Run:1;               // 运行 对应S12
        uint64_t State_Standby:1;           // 待机 对应S13
        uint64_t State_Stop:1;              // 停机 对应S14
        uint64_t Param_Fault:1;             // 故障 对应S23

        // SEG14接屏幕引脚3，分别为S27、S26、S25、S24，按COM1、COM2、COM3、COM4按顺序
        uint64_t Fault_LowVoltage:1;        // 欠压故障 对应S27
        uint64_t Fault_Overload:1;          // 过载故障 对应S26
        uint64_t Fault_Leakage:1;           // 漏电故障 对应S25
        uint64_t Fault_Null:1;              // 无故障 对应S24

        // SEG15接屏幕引脚4，分别为S28、S29、S30、S31，按COM1、COM2、COM3、COM4按顺序
        uint64_t Fault_HighVoltage:1;       // 超压功能 对应S28
        uint64_t Fault_Lackphase:1;         // 缺相故障 对应S29
        uint64_t Fault_Else:1;              // 其他故障 对应S30
        uint64_t Fault_ChangeOil:1;         // 换油功能 对应S31

    };
    struct
    {
        uint8_t  DataArray[8];
    };
    struct
    {
        uint64_t Data;
    };
} Project_DisplayData_t;

extern Project_DisplayData_t MyDisplayData;

#if 1
#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning restore
#elif defined ( __CSMC__ )
    /* anonymous unions are enabled by default */
#else
    #warning Not supported compiler type
#endif
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//--------------------------------------------------------------------------------------------------
// @函数声明调用    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9
//--------------------------------------------------------------------------------------------------
// 工程 液晶屏显示 清除内容
void Project_Display_Clear(uint8_t State);

// 工程 液晶屏显示 显示指定内容
void Project_Display_State(uint8_t State);                                  // 工程 液晶屏显示 输出状态
void Project_Display_Param(uint8_t ParamType, int32_t ParamData);           // 工程 液晶屏显示 标题数据
//void Project_ErrorDisplay(void);                                            // 工程 液晶屏显示 故障内容
void Project_Display_Mode(uint8_t Mode);                                    // 工程 液晶屏显示 模式
void Project_Display_Match(uint8_t Enable);                                 // 工程 液晶屏显示 匹配
void Project_Display_OilChange(uint8_t Enable);                             // 工程 液晶屏显示 换油
void Project_Display_NormalModeContent(uint16_t FaultType);                 // 工程 液晶屏显示 正常工作模式子模式内容
void Project_Display_ShowControlState(uint8_t WorkState);                   // 工程 液晶屏显示 控制状态

// 工程 液晶屏显示 数码管显示内容
void Project_Display_Segment_ShowString(uint8_t X,uint8_t *String);                                             // 工程 液晶屏显示 数码管 显示字符串
void Project_Display_Segment_ShowIntegerNumber(uint8_t X, int16_t IntegerNumber, uint8_t Count);                // 工程 液晶屏显示 数码管 显示数字
void Project_Display_Segment_ShowFloatNumber(uint8_t X, float FloatNumber, uint8_t Count1, uint8_t Count2);     // 工程 液晶屏显示 数码管 显示浮点数字
void Project_Display_Segment_ShowStringNumber(uint8_t X, uint8_t *String, uint16_t Number);                     // 工程 液晶屏显示 数码管 显示混合字符串与数字

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* APPLICATION_DISPLAY_H_ */

//========================================== 文 件 结 束 ===========================================

